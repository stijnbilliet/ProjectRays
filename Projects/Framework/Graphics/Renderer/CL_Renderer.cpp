#include "FrameworkPCH.h"
#include "CL_Renderer.h"

using namespace RadeonRays;

CL_Renderer::CL_Renderer()
	:SingleInstance<CL_Renderer>()
{
}

CL_Renderer::~CL_Renderer()
{
	m_pApi->DetachAll();
	IntersectionApi::Delete(m_pApi);
}

void CL_Renderer::OnInit(GameContext* pGameContext)
{
	UNREFERENCED_PARAMETER(pGameContext);

	// Choose device
	int nativeidx = -1;
	// Always use OpenCL
	IntersectionApi::SetPlatform(DeviceInfo::kOpenCL);
	for (auto idx = 0U; idx < IntersectionApi::GetDeviceCount(); ++idx)
	{
		DeviceInfo devinfo;
		IntersectionApi::GetDeviceInfo(idx, devinfo);

		if (devinfo.type == DeviceInfo::kGpu && nativeidx == -1)
		{
			nativeidx = idx;
		}

		std::stringstream openCLInfo;
		openCLInfo << "OpenCL Specification Info\nVendor: " << devinfo.vendor << "\nDevice: " << devinfo.name << "\nDeviceType: ";
		switch (devinfo.type)
		{
		case DeviceInfo::Type::kCpu:
			openCLInfo << "CPU";
			break;
		case DeviceInfo::Type::kGpu:
			openCLInfo << "GPU";
			break;
		case DeviceInfo::Type::kAccelerator:
			openCLInfo << "Accelerator";
			break;
		default:
			break;
		}

		openCLInfo << "\nPlatform: ";
		switch (devinfo.platform)
		{
		case DeviceInfo::Platform::kOpenCL:
			openCLInfo << "OpenCL";
			break;
		case DeviceInfo::Platform::kEmbree:
			openCLInfo << "Embree";
			break;
		case DeviceInfo::Platform::kVulkan:
			openCLInfo << "Vulkan";
			break;
		case DeviceInfo::Platform::kAny:
		default:
			openCLInfo << "Any";
			break;
		}
		openCLInfo << '\n';
		Logger::GetInstance().LogInfo(openCLInfo.str());
	}
	assert(nativeidx != -1);

	//create intersection api
	m_pApi = IntersectionApi::Create(nativeidx);
}

void CL_Renderer::PreDraw(GameContext* pContext) const
{
	UNREFERENCED_PARAMETER(pContext);
}

void CL_Renderer::OnDraw(GameContext* pContext) const
{
	UNREFERENCED_PARAMETER(pContext);
}

IntersectionApi * CL_Renderer::GetRaysAPI() const
{
	return m_pApi;
}