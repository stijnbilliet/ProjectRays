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

inline void CL_ERROR_CHECK(int status, const std::string& errorMessage) {
	if (status != 0)
	{
		Logger::GetInstance().Logf(ELogLevel::ERR0R, (errorMessage + "%i").c_str(), status);
		throw(std::runtime_error(errorMessage));
	}
	status = 0;
};

using namespace RadeonRays;

CL_Renderer::CL_Renderer()
	:SingleInstance<CL_Renderer>()
    ,m_AngularExtent(2.0f)
    ,m_TileSize(4)
{
}

CL_Renderer::~CL_Renderer()
{
    delete m_TempLightBuffer;
	m_pRRContext->DetachAll();
	IntersectionApi::Delete(m_pRRContext);
}

void CL_Renderer::OnInit(GameContext*)
{
	PropertyManager::GetInstance().GetInt("renderer_viewwidth", m_ScreenWidth);
	PropertyManager::GetInstance().GetInt("renderer_viewheight", m_ScreenHeight);
	PropertyManager::GetInstance().GetString("assetpath", m_AssetPath);

	_putenv("GPU_MAX_ALLOC_PERCENT=100");
}

void CL_Renderer::OnUpdate(GameContext*)
{
}

IntersectionApi* CL_Renderer::GetRaysAPI() const
{
	return m_pRRContext;
}

void CL_Renderer::InitCL(GameContext*)
{
	//Get platform and device information
	cl_platform_id platformId = NULL;
	cl_device_id deviceId = NULL;
	cl_uint nmbDevices;
	cl_uint nmbPlatforms;
	cl_int status = CL_SUCCESS;

	status = clGetPlatformIDs(1, &platformId, &nmbPlatforms);
	CL_ERROR_CHECK(status, "Could not fetch platforms");

	status = clGetDeviceIDs(platformId, CL_DEVICE_TYPE_GPU, 1, &deviceId, &nmbDevices);
	CL_ERROR_CHECK(status, "Could not fetch devices");

	//Fetch device from device id
	m_CLDevice = CLWDevice::Create(deviceId);

	//Create openCL/openGL interop context
	cl_context_properties ccp[] = {
		CL_GL_CONTEXT_KHR, (cl_context_properties)getCurrentGLContext(),
		CL_WGL_HDC_KHR, (cl_context_properties)getCurrentGLDrawable(),
		CL_CONTEXT_PLATFORM, (cl_context_properties)platformId,
		0
	};
	m_CLContext = CLWContext::Create(m_CLDevice, ccp);
}

void CL_Renderer::InitRadeonRays(GameContext*)
{
	//Init RadeonRays from previously created openCL context
	cl_device_id id = m_CLDevice.GetID();
	cl_command_queue queue = m_CLContext.GetCommandQueue(0);

	m_pRRContext = RadeonRays::CreateFromOpenClContext(m_CLContext, id, queue);
}

void CL_Renderer::InitKernels(GameContext* pGameContext)
{
	//Build Shadowrays kernel
	const char* kBuildopts("-Werror -cl-mad-enable -cl-fast-relaxed-math -cl-std=CL2.0 -I . ");
	std::string kernelPath = m_AssetPath + "/" + "Kernels/RayGenerator.cl";
	m_RayGenerator = CLWProgram::CreateFromFile(kernelPath.c_str(), kBuildopts, m_CLContext);

	InitShadowRaysKernel(pGameContext);
	InitLightMaskKernel(pGameContext);
    InitSampleKernel(pGameContext);
}

void CL_Renderer::GenerateShadowRays(GameContext* pGameContext)
{
	auto commandQueue = m_CLContext.GetCommandQueue(0);
	cl_int status = CL_SUCCESS;
	auto dirLightPos = pGameContext->m_pGLRenderer->GetDirectionalLightPos();
	cl_float4 light_cl = { dirLightPos.x, dirLightPos.y, dirLightPos.z, 1.0f };
    size_t gs[] = { (size_t)m_ScreenWidth, (size_t)m_ScreenHeight };

	//Acquire WorldPosBuffer
	status = clEnqueueAcquireGLObjects(commandQueue, 1, &m_CLGLWorldPosBuffer, 0, nullptr, nullptr);
	CL_ERROR_CHECK(status, "clEnqueueAcquireGLObjects failed (m_CLGLWorldPosBuffer)");

	//Acquire normalBuffer
	status = clEnqueueAcquireGLObjects(commandQueue, 1, &m_CLGLNormalBuffer, 0, nullptr, nullptr);
	CL_ERROR_CHECK(status, "clEnqueueAcquireGLObjects failed (m_CLGLNormalBuffer)");

	//Send new information to shader
    m_ShadowRayGenerator.SetArg(0, sizeof(cl_float4), &light_cl);
    m_ShadowRayGenerator.SetArg(1, sizeof(cl_float), &m_AngularExtent);
    m_ShadowRayGenerator.SetArg(2, sizeof(cl_uint), &m_TileSize);
	m_ShadowRayGenerator.SetArg(3, sizeof(cl_mem), &m_CLRRRaysBuffer);
	m_ShadowRayGenerator.SetArg(4, sizeof(cl_mem), &m_CLGLWorldPosBuffer);
	m_ShadowRayGenerator.SetArg(5, sizeof(cl_mem), &m_CLGLNormalBuffer);

	//Launch kernels on cuda cores
	status = clEnqueueNDRangeKernel(m_CLContext.GetCommandQueue(0), m_ShadowRayGenerator, 2, NULL, gs, NULL, 0, nullptr, nullptr);
	CL_ERROR_CHECK(status, "clEnqueueNDRangeKernel failed (m_ShadowRayGenerator)");
	
	//Querry kernel execution
	status = clFlush(commandQueue);
	CL_ERROR_CHECK(status, "Flush failed (m_ShadowRayGenerator)");

	//Release worldposBuffer
	status = clEnqueueReleaseGLObjects(commandQueue, 1, &m_CLGLWorldPosBuffer, 0, nullptr, nullptr);
	CL_ERROR_CHECK(status, "clEnqueueReleaseGLObjects failed (m_CLGLWorldPosBuffer)");

	//Release normalBuffer
	status = clEnqueueReleaseGLObjects(commandQueue, 1, &m_CLGLNormalBuffer, 0, nullptr, nullptr);
	CL_ERROR_CHECK(status, "clEnqueueReleaseGLObjects failed (m_CLGLNormalBuffer)");
}

void CL_Renderer::GenerateLightingMask(GameContext*)
{
	auto commandQueue = m_CLContext.GetCommandQueue(0);
	cl_int status = CL_SUCCESS;
	size_t gs[] = { (size_t)m_ScreenWidth, (size_t)m_ScreenHeight };

	//Acquire lightBuffer
	status = clEnqueueAcquireGLObjects(commandQueue, 1, &m_CLGLLightBuffer, 0, nullptr, nullptr);
	CL_ERROR_CHECK(status, "clEnqueueReleaseGLObjects failed (m_CLGLLightBuffer)");

	//Get kernel from kernelprogram
	m_LightMaskGenerator.SetArg(0, sizeof(cl_mem), &m_CLRROcclusionBuffer);
	m_LightMaskGenerator.SetArg(1, sizeof(cl_mem), &m_CLGLLightBuffer);

	//Launch kernels on cuda cores
	status = clEnqueueNDRangeKernel(commandQueue, m_LightMaskGenerator, 2, NULL, gs, NULL, 0, nullptr, nullptr);
	CL_ERROR_CHECK(status, "clEnqueueNDRangeKernel failed (m_LightMaskGenerator)");

	status = clFlush(commandQueue);
	CL_ERROR_CHECK(status, "Flush failed (m_LightMaskGenerator)");

	//Release LightBuffer
	status = clEnqueueReleaseGLObjects(commandQueue, 1, &m_CLGLLightBuffer, 0, nullptr, nullptr);
	CL_ERROR_CHECK(status, "clEnqueueReleaseGLObjects failed (m_CLGLLightBuffer)");
}

void CL_Renderer::SampleShadows(GameContext*)
{
    auto commandQueue = m_CLContext.GetCommandQueue(0);
    cl_int status = CL_SUCCESS;
    size_t gs[] = { (size_t)m_ScreenWidth, (size_t)m_ScreenHeight };
    size_t ls[] = { (size_t)m_TileSize, (size_t)m_TileSize };

    //Acquire lightBuffer
    status = clEnqueueAcquireGLObjects(commandQueue, 1, &m_CLGLLightBuffer, 0, nullptr, nullptr);
    CL_ERROR_CHECK(status, "clEnqueueReleaseGLObjects failed (m_CLGLLightBuffer)");

    //Blit lightmask
    size_t origin[] = { 0, 0, 0 };
    size_t region[] = { (size_t)m_ScreenWidth, (size_t)m_ScreenHeight, (size_t)1 };
    status = clEnqueueCopyImageToBuffer(commandQueue, m_CLGLLightBuffer, m_CLTempLightBuffer, origin, region, 0, 0, nullptr, nullptr);
    CL_ERROR_CHECK(status, "clEnqueueCopyBufferToImage failed (m_CLGLLightBuffer | m_CLTempLightBuffer)");

    //Get kernel from kernelprogram
    m_SoftSampleKernel.SetArg(0, sizeof(cl_uint), &m_TileSize);
    m_SoftSampleKernel.SetArg(1, sizeof(cl_mem), &m_CLTempLightBuffer);
    m_SoftSampleKernel.SetArg(2, sizeof(cl_mem), &m_CLGLLightBuffer);

    //Launch kernels on cuda cores
    status = clEnqueueNDRangeKernel(commandQueue, m_SoftSampleKernel, 2, NULL, gs, ls, 0, nullptr, nullptr);
    CL_ERROR_CHECK(status, "clEnqueueNDRangeKernel failed (m_SoftSampleKernel)");

    status = clFlush(commandQueue);
    CL_ERROR_CHECK(status, "Flush failed (m_SoftSampleKernel)");

    //Release LightBuffer
    status = clEnqueueReleaseGLObjects(commandQueue, 1, &m_CLGLLightBuffer, 0, nullptr, nullptr);
    CL_ERROR_CHECK(status, "clEnqueueReleaseGLObjects failed (m_CLGLLightBuffer)");
}

void CL_Renderer::InitShadowRaysKernel(GameContext * pGameContext)
{
	//Fetch buffer id's
	auto pGLRenderer = pGameContext->m_pGLRenderer;
	auto gWorldPosBuffer = pGLRenderer->GetWorldPosBuffer();
	auto gNormalBuffer = pGLRenderer->GetNormalBuffer();
	cl_int status = CL_SUCCESS;

	//Init Buffers
	m_CLGLWorldPosBuffer = clCreateFromGLTexture(m_CLContext, CL_MEM_READ_ONLY, GL_TEXTURE_2D, 0, gWorldPosBuffer, &status);
	CL_ERROR_CHECK(status, "Texture sharing failed (gPosition buffer)");

	m_CLGLNormalBuffer = clCreateFromGLTexture(m_CLContext, CL_MEM_READ_ONLY, GL_TEXTURE_2D, 0, gNormalBuffer, &status);
	CL_ERROR_CHECK(status, "Texture sharing failed (gNormal buffer)");

	m_RayData = new RadeonRays::ray[m_ScreenWidth*m_ScreenHeight]();
	m_CLRRRaysBuffer = clCreateBuffer(m_CLContext, CL_MEM_READ_WRITE, m_ScreenWidth*m_ScreenHeight * sizeof(RadeonRays::ray), m_RayData, &status);
	CL_ERROR_CHECK(status, "Buffer creation failed (rays buffer)");

	m_OcclusionData = new RadeonRays::Intersection[m_ScreenWidth*m_ScreenHeight]();
	m_CLRROcclusionBuffer = clCreateBuffer(m_CLContext, CL_MEM_READ_WRITE, m_ScreenWidth*m_ScreenHeight * sizeof(RadeonRays::Intersection), m_OcclusionData, &status);
	CL_ERROR_CHECK(status, "Buffer creation failed (occlusion buffer)");

	//Init kernel
	m_ShadowRayGenerator = m_RayGenerator.GetKernel("GenerateShadowRay");
}

void CL_Renderer::InitLightMaskKernel(GameContext* pGameContext)
{
	auto pGLRenderer = pGameContext->m_pGLRenderer;
	auto gLightBuffer = pGLRenderer->GetLightBuffer();
	cl_int status = CL_SUCCESS;
	
	//Init buffers
	m_CLGLLightBuffer = clCreateFromGLTexture(m_CLContext, CL_MEM_WRITE_ONLY, GL_TEXTURE_2D, 0, gLightBuffer, &status);
	CL_ERROR_CHECK(status, "Texture sharing failed (gLightBuffer buffer)");

	//Init kernel
	m_LightMaskGenerator = m_RayGenerator.GetKernel("GenerateLightingMask");
}

void CL_Renderer::InitSampleKernel(GameContext* /*pGameContext*/)
{
    //Init buffers
    cl_int status = CL_SUCCESS;
    m_TempLightBuffer = new cl_float4[m_ScreenWidth*m_ScreenHeight * sizeof(cl_float4)];
    m_CLTempLightBuffer = clCreateBuffer(m_CLContext, CL_MEM_WRITE_ONLY, m_ScreenWidth*m_ScreenHeight*sizeof(cl_float4), m_TempLightBuffer, &status);
    CL_ERROR_CHECK(status, "Temp light buffer creation failed");

    //Init kernel
    m_SoftSampleKernel = m_RayGenerator.GetKernel("SoftShadowSample");
}

void CL_Renderer::RaytracedShadows(GameContext* pGameContext)
{
	auto commandQueue = m_CLContext.GetCommandQueue(0);

	//Generate shadow rays
	GenerateShadowRays(pGameContext);

	//Fetch rays and occlusion information
	m_RaysBuffer = CreateFromOpenClBuffer(m_pRRContext, m_CLRRRaysBuffer);
	m_OcclusionBuffer = CreateFromOpenClBuffer(m_pRRContext, m_CLRROcclusionBuffer);

	//Query occlusion
	RadeonRays::Event* e = nullptr;
	m_pRRContext->QueryIntersection(m_RaysBuffer, m_ScreenWidth*m_ScreenHeight, m_OcclusionBuffer, nullptr, &e);
	e->Wait();
	
	//Generate lightmap
	GenerateLightingMask(pGameContext);

    //Sample soft shadows
    SampleShadows(pGameContext);
}