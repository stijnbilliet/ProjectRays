#include "FrameworkPCH.h"
#include "RayShapeComponent.h"

using namespace RadeonRays;

int RayShapeComponent::_ShapeID = -1;

RayShapeComponent::RayShapeComponent(const Mesh* pMesh)
	:BaseComponent(), m_pMeshData(pMesh), m_pShape(nullptr)
{
}

RayShapeComponent::~RayShapeComponent()
{
}

void RayShapeComponent::OnInit(GameContext* pContext)
{
	UNREFERENCED_PARAMETER(pContext);

	//get mesh data
	if (!m_pMeshData)
	{
		Logger::GetInstance().LogError("No meshdata was found while trying to create Rayshapecomponent\n");
		return;
	}

	//get all vertex positions from meshfilter
	std::vector<glm::vec3> verticesVect = {};
	auto verticesData = m_pMeshData->GetVertices();

	for (auto vertex : verticesData)
	{
		verticesVect.push_back(vertex.Position);
	}

	if (verticesData.size() == 0 || verticesVect.size() == 0)
	{
		Logger::GetInstance().LogError("No vertices found while trying to create Rayshapecomponent\n");
		return;
	}

	//get indices from meshfilter
	auto indicesVect = m_pMeshData->GetIndices();

	if (indicesVect.size() == 0)
	{
		Logger::GetInstance().LogInfo("No vertices found while trying to create Rayshapecomponent\n");
		return;
	}

	//store important data
	float* vertices = &(verticesVect.data()[0].x);
	auto nmbVerts = (unsigned int)verticesVect.size();
	int* indices = (int*)indicesVect.data();
	auto nmbIndices = (unsigned int)indicesVect.size();
	auto nmbfaces = nmbIndices / 3;

	//get rays api from renderer
	auto pRaysAPI = pContext->m_pCLRenderer->GetRaysAPI();
	if (!pRaysAPI)
	{
		Logger::GetInstance().LogInfo("Rays api was nullptr, no shape was created\n");
		return;
	}

	//create Radeon Rays shape
	m_pShape = pRaysAPI->CreateMesh(vertices, nmbVerts, 3 * sizeof(float), indices, 0, nullptr, nmbfaces);
	assert(m_pShape != nullptr);
	pRaysAPI->AttachShape(m_pShape);
	_ShapeID++;
	m_pShape->SetId(_ShapeID);

	pRaysAPI->Commit();
}

void RayShapeComponent::OnUpdate(GameContext * pContext)
{
	UNREFERENCED_PARAMETER(pContext);

	auto transformComponent = m_pAttachedTo->GetComponent<TransformComponent>();
	if (transformComponent && transformComponent->WasTranslated())
	{
		auto owf = transformComponent->GetWorldMatrix();
		auto shapeWorldTransform = matrix(
			owf[0][0], owf[0][1], owf[0][2], owf[0][3],
			owf[1][0], owf[1][1], owf[1][2], owf[1][3],
			owf[2][0], owf[2][1], owf[2][2], owf[2][3],
			owf[3][0], owf[3][1], owf[3][2], owf[3][3]
		);
		m_pShape->SetTransform(shapeWorldTransform, inverse(shapeWorldTransform));
	}
}

void RayShapeComponent::OnDraw(GameContext * pContext) const
{
	UNREFERENCED_PARAMETER(pContext);
}