#include "FrameworkPCH.h"
#include "CL_Renderer.h"

using namespace RadeonRays;

CL_Renderer::CL_Renderer()
	:SingleInstance<CL_Renderer>()
{
}

CL_Renderer::~CL_Renderer()
{
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
	}
	assert(nativeidx != -1);

	//create intersection api
	m_pApi = IntersectionApi::Create(nativeidx);

	//fill shapes buffer
	unsigned int shapeId = 0;
	auto pScene = dynamic_cast<const Scene*>(pGameContext->m_pSceneManager->GetActive());
	for (auto pObj : pScene->GetObjects())
	{
		auto meshComp = pObj->GetComponent<MeshDrawComponent>();
		if (meshComp)
		{
			//get mesh data from meshcomp
			auto pMeshData = meshComp->GetMeshData();
			if (!pMeshData) return;

			//get all vertex positions from meshfilter
			std::vector<glm::vec3> verticesVect = {};
			auto verticesData = pMeshData->GetVertices();
			if (verticesData.size() == 0) return;

			for (auto vertex : verticesData)
			{
				verticesVect.push_back(vertex.Position);
			}
			if (verticesVect.size() == 0) return;

			//get indices from meshfilter
			auto indicesVect = pMeshData->GetIndices();

			//store important data
			float* vertices = &(verticesVect.data()[0].x);
			auto nmbVerts = (unsigned int)verticesVect.size();
			int* indices = (int*)indicesVect.data();
			auto nmbIndices = (unsigned int)indicesVect.size();
			auto nmbfaces = nmbIndices / 3;

			//std::cout << &vertices << "\t" <<&indices << '\t' << nmbVerts << '\t' << nmbIndices <<  '\n';
			Shape* shape = m_pApi->CreateMesh(vertices, nmbVerts, 3 * sizeof(float), indices, 0, nullptr, nmbfaces);
			assert(shape != nullptr);
			m_pApi->AttachShape(shape);
			shape->SetId(shapeId);
			shapeId++;
		}
	}

	// Commit scene changes
	m_pApi->Commit();

	int viewWidth, viewHeight{};
	PropertyManager::GetInstance().GetInt("renderer_viewwidth", viewWidth);
	PropertyManager::GetInstance().GetInt("renderer_viewheight", viewHeight);

	const int k_raypack_size = viewWidth * viewHeight;
}

void CL_Renderer::PreDraw(GameContext* pContext) const
{
	UNREFERENCED_PARAMETER(pContext);
}

void CL_Renderer::OnDraw(GameContext* pContext) const
{
	UNREFERENCED_PARAMETER(pContext);
}