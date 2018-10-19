#include "FrameworkPCH.h"
#include "CL_Renderer.h"

#if defined(_WIN32)
#include <windows.h>
#include <wingdi.h>
typedef HGLRC GLContext;
typedef HDC   GLDrawable;
typedef HWND  GLWindow;

GLContext getCurrentGLContext(void) { return wglGetCurrentContext(); }
GLDrawable getCurrentGLDrawable(void) { return wglGetCurrentDC(); }
GLWindow getCurrentGLWindow(void) { return WindowFromDC(wglGetCurrentDC()); }
#elif defined(__unix__)
/* FIXME: consider Wayland or a EGL environment */
typedef GLXContext GLContext;
typedef GLXDrawable GLDrawable;
typedef Window GLWindow;

GLContext getCurrentGLContext(void) { return glXGetCurrentContext(); }
GLDrawable getCurrentGLDrawable(void) { return glXGetCurrentDrawable(); }
GLWindow getCurrentGLWindow(void) { return glXGetCurrentDrawable(); }
#endif

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

	//Get platform and device information
	cl_platform_id platform_id = NULL;
	cl_device_id device_id = NULL;

	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;

	cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	if (ret != 0) Logger::GetInstance().Logf(ELogLevel::ERR0R, "Tried to fetch platform id's but %i", ret);

	ret = clGetDeviceIDs(
		platform_id,
		CL_DEVICE_TYPE_DEFAULT, //Fetch all openCL capable devices, but prefer GPU
		1,
		&device_id, &ret_num_devices
	);
	if (ret != 0) Logger::GetInstance().Logf(ELogLevel::ERR0R, "Tried to fetch device id's but %i", ret);

	//Fetch device from device id
	m_CLDevice = CLWDevice::Create(device_id);

	//Create openCL/openGL interop context
	cl_context_properties ccp[] = {
		CL_GL_CONTEXT_KHR, (cl_context_properties)getCurrentGLContext(),
		CL_WGL_HDC_KHR, (cl_context_properties)getCurrentGLDrawable(),
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
	//Build Shadowrays kernel
	const char* kBuildopts(" -cl-mad-enable -cl-fast-relaxed-math -cl-std=CL1.2 -I . ");
	std::string kernelPath = m_AssetPath + "/" + "Kernels/RayGenerator.cl";
	m_RayGenerator = CLWProgram::CreateFromFile(kernelPath.c_str(), kBuildopts, m_CLContext);

	InitShadowKernel(pGameContext);
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

	//Launch kernels on cuda cores
	cl_int status = CL_SUCCESS;
	status = clEnqueueNDRangeKernel(m_CLContext.GetCommandQueue(0), m_ShadowRayGenerator, 2, NULL, gs, NULL, 0, nullptr, nullptr);
	if (status != CL_SUCCESS)
	{
		Logger::GetInstance().Logf(ELogLevel::ERR0R, "clEnqueueNDRangeKernel failed %i\n", status);
		return;
	}
	m_CLContext.Flush(0);

	//Fetch results
	//TODO MIGHT BE CORRUPT BECAUSE RAY BUFFERS ARE NOT SAME SIZE
	m_RRaysBuffer = CreateFromOpenClBuffer(m_pRRContext, m_CLGLRaysBuffer);
}

void CL_Renderer::InitShadowKernel(GameContext * pGameContext)
{
	//Fetch buffer id's
	auto pGLRenderer = pGameContext->m_pGLRenderer;
	auto gWorldPosBuffer = pGLRenderer->GetWorldPosBuffer();
	auto gNormalBuffer = pGLRenderer->GetNormalBuffer();

	//Init Buffers
	int status = 0;
	m_CLGLWorldPosBuffer = clCreateFromGLTexture(m_CLContext, CL_MEM_READ_ONLY, GL_TEXTURE_2D, 0, gWorldPosBuffer, &status);
	if(status != 0) Logger::GetInstance().Logf(ELogLevel::ERR0R, "Texture sharing failed (gPosition buffer) %i", status);

	status = 0;
	m_CLGLNormalBuffer = clCreateFromGLTexture(m_CLContext, CL_MEM_READ_ONLY, GL_TEXTURE_2D, 0, gNormalBuffer, &status);
	if (status != 0) Logger::GetInstance().Logf(ELogLevel::ERR0R, "Texture sharing failed (gNormal buffer) %i", status);

	status = 0;
	m_CLGLRaysBuffer = clCreateBuffer(m_CLContext, CL_MEM_READ_WRITE, m_ScreenWidth*m_ScreenHeight * sizeof(RadeonRays::ray), nullptr, &status);
	if (status != 0) Logger::GetInstance().Logf(ELogLevel::ERR0R, "Buffer creation failed (rays buffer) %i", status);

	m_OcclusionBuffer = m_pRRContext->CreateBuffer(m_ScreenWidth*m_ScreenHeight*sizeof(int), nullptr);

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
	auto commandQueue = m_CLContext.GetCommandQueue(0);
	clEnqueueAcquireGLObjects(commandQueue, 1, &m_CLGLWorldPosBuffer, 0, nullptr, nullptr);
	clEnqueueAcquireGLObjects(commandQueue, 1, &m_CLGLNormalBuffer, 0, nullptr, nullptr);
	{
		GenerateShadowRays(pGameContext);
	}
	clEnqueueReleaseGLObjects(commandQueue, 1, &m_CLGLWorldPosBuffer, 0, nullptr, nullptr);
	clEnqueueReleaseGLObjects(commandQueue, 1, &m_CLGLNormalBuffer, 0, nullptr, nullptr);
	clFinish(m_CLContext.GetCommandQueue(0));

	//Query occlusion
	m_pRRContext->QueryOcclusion(m_RRaysBuffer, m_ScreenWidth*m_ScreenHeight, m_OcclusionBuffer, nullptr, nullptr);

	//Get results

	//Copy results

}