#include "FrameworkPCH.h"
#include "CL_Renderer.h"

#include <CL/cl.h>
#include "radeon_rays.h"

using namespace RadeonRays;

CL_Renderer::CL_Renderer()
	:SingleInstance<CL_Renderer>()
{
}

CL_Renderer::~CL_Renderer()
{
}

void CL_Renderer::OnInit()
{
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
	}

	//assert(nativeidx != -1);
	//IntersectionApi* api = IntersectionApi::Create(nativeidx);
}
