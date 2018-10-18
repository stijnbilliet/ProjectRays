#include "FrameworkPCH.h"
#include "CL_Renderer.h"

using namespace RadeonRays;

CL_Renderer::CL_Renderer()
	:SingleInstance<CL_Renderer>()
{
}

CL_Renderer::~CL_Renderer()
{
	m_pRRContext->DetachAll();
	IntersectionApi::Delete(m_pRRContext);
	//TODO RELEASE OPENGL OBJECTS
}

void CL_Renderer::OnInit(GameContext* pGameContext)
{
	UNREFERENCED_PARAMETER(pGameContext);

	PropertyManager::GetInstance().GetInt("renderer_viewwidth", m_ScreenWidth);
	PropertyManager::GetInstance().GetInt("renderer_viewheight", m_ScreenHeight);
	PropertyManager::GetInstance().GetString("assetpath", m_AssetPath);

	InitCL(pGameContext);
	InitRadeonRays(pGameContext);
	InitKernels(pGameContext);
}

void CL_Renderer::OnUpdate(GameContext * pContext)
{
	RaytracedShadows(pContext);
}

IntersectionApi* CL_Renderer::GetRaysAPI() const
{
	return m_pRRContext;
}

void CL_Renderer::InitCL(GameContext* pGameContext)
{
	UNREFERENCED_PARAMETER(pGameContext);

	//Fetch context and window from openGL renderer
	auto glContext = pGameContext->m_pGLRenderer->GetGLContext();
	auto glWindow = pGameContext->m_pGLRenderer->GetWindow();

	//Get platform and device information
	cl_platform_id platform_id = NULL;
	cl_device_id device_id = NULL;

	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;

	cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	ret = clGetDeviceIDs(
		platform_id,
		CL_DEVICE_TYPE_DEFAULT, //Fetch all openCL capable devices, but prefer GPU
		1,
		&device_id, &ret_num_devices
	);

	//Fetch device from device id
	m_CLDevice = CLWDevice::Create(device_id);

	//Create openCL/openGL interop context
	cl_context_properties ccp[] = {
		CL_GL_CONTEXT_KHR, (cl_context_properties)glContext,
		CL_WGL_HDC_KHR, (cl_context_properties)glWindow,
		CL_CONTEXT_PLATFORM, (cl_context_properties)platform_id,
		0
	};
	m_CLContext = CLWContext::Create(m_CLDevice, ccp);

	//Print Device Info
	std::stringstream clInfo;
	clInfo << "OpenCL Specification Info\nVendor: " << m_CLDevice.GetVendor() << "\nDevice: " << m_CLDevice.GetName() << "\nVersion: " << m_CLDevice.GetVersion() << '\n';
	Logger::GetInstance().LogInfo(clInfo.str());
}

void CL_Renderer::InitRadeonRays(GameContext* pGameContext)
{
	UNREFERENCED_PARAMETER(pGameContext);

	//Init RadeonRays from previously created openCL context
	cl_device_id id = m_CLDevice.GetID();
	cl_command_queue queue = m_CLContext.GetCommandQueue(0);

	m_pRRContext = RadeonRays::CreateFromOpenClContext(m_CLContext, id, queue);
}

void CL_Renderer::InitKernels(GameContext* pGameContext)
{
	const char* kBuildopts(" -cl-mad-enable -cl-fast-relaxed-math -cl-std=CL1.2 -I . ");
	int status = 0;

	InitShadowKernel(pGameContext, kBuildopts, &status);
	if (status != 0) Logger::GetInstance().LogError("Failed to share one/more buffer(s) (OpenCL/OpenGL interop)!\n");
}

void CL_Renderer::GenerateShadowRays(GameContext * pGameContext)
{
	UNREFERENCED_PARAMETER(pGameContext);

	//Fetch new light position
	auto dirLightPos = pGameContext->m_pGLRenderer->GetDirectionalLightPos();
	cl_float4 light_cl = { dirLightPos.x, dirLightPos.y, dirLightPos.z, 1.0f };

	//Send new information to shader
	m_ShadowRayGenerator.SetArg(2, light_cl);

	//Determine global size (nmb of workgroups)
	size_t gs[] = { (size_t)m_ScreenWidth, (size_t)m_ScreenHeight };

	//Determine local size (nmb workgroups per thread)
	size_t ls[] = { 1, 1 };

	//Launch kernels on cuda cores
	m_CLContext.Launch2D(0, gs, ls, m_ShadowRayGenerator);
	m_CLContext.Flush(0);

	//Fetch results
	m_RRaysBuffer = CreateFromOpenClBuffer(m_pRRContext, m_CLGLRaysBuffer);
}

void CL_Renderer::InitShadowKernel(GameContext * pGameContext, const char * buildOpts, int* status)
{
	//Build Shadowrays kernel
	std::string kernelPath = m_AssetPath + "/" + "Kernels/RayGenerator.cl";
	m_RayGenerator = CLWProgram::CreateFromFile(kernelPath.c_str(), buildOpts, m_CLContext);

	//Fetch buffer id's
	auto pGLRenderer = pGameContext->m_pGLRenderer;
	auto gWorldPosBuffer = pGLRenderer->GetWorldPosBuffer();
	auto gNormalBuffer = pGLRenderer->GetNormalBuffer();

	//Init Buffers
	m_CLGLWorldPosBuffer = clCreateFromGLTexture(m_CLContext, CL_MEM_READ_ONLY, GL_TEXTURE_2D, 0, gWorldPosBuffer, status);
	m_CLGLNormalBuffer = clCreateFromGLTexture(m_CLContext, CL_MEM_READ_ONLY, GL_TEXTURE_2D, 0, gNormalBuffer, status);

	m_CLGLRaysBuffer = clCreateBuffer(m_CLContext, CL_MEM_READ_WRITE, m_ScreenWidth*m_ScreenHeight * sizeof(RadeonRays::ray), nullptr, status);

	auto dirLightPos = pGameContext->m_pGLRenderer->GetDirectionalLightPos();
	cl_float4 light_cl = { dirLightPos.x, dirLightPos.y, dirLightPos.z, 1.0f };

	//Get kernel from kernelprogram
	m_ShadowRayGenerator = m_RayGenerator.GetKernel("GenerateShadowRays");
	m_ShadowRayGenerator.SetArg(0, m_ScreenWidth);
	m_ShadowRayGenerator.SetArg(1, m_ScreenHeight);
	m_ShadowRayGenerator.SetArg(2, light_cl);
	m_ShadowRayGenerator.SetArg(3, sizeof(cl_mem), &m_CLGLRaysBuffer);
	m_ShadowRayGenerator.SetArg(4, sizeof(cl_mem), &m_CLGLWorldPosBuffer);
	m_ShadowRayGenerator.SetArg(5, sizeof(cl_mem), &m_CLGLNormalBuffer);
}

void CL_Renderer::RaytracedShadows(GameContext* pGameContext)
{
	//Prepare rays
	GenerateShadowRays(pGameContext);

	//Query occlusion

	//Get results

	//Copy results

}