#include "FrameworkPCH.h"
#include "CL_Renderer.h"

using namespace RadeonRays;

CL_Renderer::CL_Renderer()
	:SingleInstance<CL_Renderer>()
{
	PropertyManager::GetInstance().GetInt("renderer_viewwidth", m_ScreenWidth);
	PropertyManager::GetInstance().GetInt("renderer_viewheight", m_ScreenHeight);
}

CL_Renderer::~CL_Renderer()
{
	m_pApi->DetachAll();
	IntersectionApi::Delete(m_pApi);
}

void CL_Renderer::OnInit(GameContext* pGameContext)
{
	UNREFERENCED_PARAMETER(pGameContext);

	InitCL(pGameContext);
	InitRadeonRays(pGameContext);
	InitKernels(pGameContext);
}

void CL_Renderer::PreDraw(GameContext* pContext) const
{
	UNREFERENCED_PARAMETER(pContext);
}

void CL_Renderer::OnDraw(GameContext* pContext) const
{
	UNREFERENCED_PARAMETER(pContext);

	//Prepare rays


	//Query intersection


	//Get results


	//Copy results

}

IntersectionApi* CL_Renderer::GetRaysAPI() const
{
	return m_pApi;
}

void CL_Renderer::InitCL(GameContext* pGameContext)
{
	UNREFERENCED_PARAMETER(pGameContext);

	auto pGLRenderer = pGameContext->m_pGLRenderer;

	cl_platform_id platform_id = NULL;
	cl_device_id device_id = NULL;

	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;

	cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);

	m_CLDevice = CLWDevice::Create(device_id);
	cl_context_properties ccp[] = {
		CL_GL_CONTEXT_KHR, (cl_context_properties)pGLRenderer->GetGLContext(),
		CL_WGL_HDC_KHR, (cl_context_properties)pGLRenderer->GetWindow(),
		CL_CONTEXT_PLATFORM, (cl_context_properties)platform_id,
		0
	};

	m_CLContext = CLWContext::Create(m_CLDevice, ccp);

	//get device info
	std::stringstream clInfo;
	clInfo << "OpenCL Specification Info\nVendor: " << m_CLDevice.GetVendor() << "\nDevice: " << m_CLDevice.GetName() << "\nVersion: " << m_CLDevice.GetVersion() << '\n';
	Logger::GetInstance().LogInfo(clInfo.str());
}

void CL_Renderer::InitRadeonRays(GameContext* pGameContext)
{
	UNREFERENCED_PARAMETER(pGameContext);

	//**Init RadeonRays
	cl_device_id id = m_CLDevice.GetID();
	cl_command_queue queue = m_CLContext.GetCommandQueue(0);

	// Create intersection API
	m_pApi = RadeonRays::CreateFromOpenClContext(m_CLContext, id, queue);
	m_Rays.reserve(m_ScreenWidth * m_ScreenHeight);
}

void CL_Renderer::InitKernels(GameContext* pGameContext)
{
	auto pGLRenderer = pGameContext->m_pGLRenderer;

	PropertyManager::GetInstance().GetString("assetpath", m_AssetPath);
	const char* kBuildopts(" -cl-mad-enable -cl-fast-relaxed-math -cl-std=CL1.2 -I . ");
	std::string kernelPath = m_AssetPath + "/" + "Kernels/ShadowRays.cl";
	m_ShadowProgram = CLWProgram::CreateFromFile(kernelPath.c_str(), kBuildopts, m_CLContext);

	int status = 0;
	auto gWorldPosBuffer = pGLRenderer->GetWorldPosBuffer();
	auto test = clCreateFromGLTexture(m_CLContext, CL_MEM_READ_ONLY, GL_TEXTURE_2D, 0, gWorldPosBuffer, &status);
	if (status == 0)
	{
		printf("Successfully shared!\n");
	}
	else
	{
		printf("Failed to share%i!\n", status);
	}
}