#include "GamePCH.h"
#include "SponzaScene.h"
#include "Gamemode/GMSandbox.h"

using Super = Scene;

SponzaScene::SponzaScene()
	:Scene("SponzaScene"), m_SunAngle()
{
}


SponzaScene::~SponzaScene()
{
}

void SponzaScene::PreInit(GameContext * pContext)
{
	Super::OnInit(pContext);

	auto pSceneObj = new GameObject();
	Add(pSceneObj);

	auto pShaderProgram = new ShaderProgram("Shaders/Deferred_Geometry.vs", "Shaders/Deferred_Geometry.fs");
	auto pModel = new Model("Models/Sponza/sponza.obj");
	m_Meshes = pModel->GetMeshes();
	for (size_t i = 0; i < m_Meshes.size(); i++)
	{
		auto pChildObj = new GameObject();
		pSceneObj->Add(pChildObj);

		Mesh* pMesh = &m_Meshes[i];

		auto pMeshDrawComponent = new MeshDrawComponent(pMesh, pShaderProgram);
		auto pRaytraceComponent = new RayShapeComponent(pMesh);
		pChildObj->AddComponent(pMeshDrawComponent);
		pChildObj->AddComponent(pRaytraceComponent);
	}
}

void SponzaScene::PostInit(GameContext * pContext)
{
	Super::PostInit(pContext);
	pContext->m_pActiveCamera->GetGameObject()->GetTransform()->Translate(-55.0f, 15.0f, 0.0f);
	pContext->m_pActiveCamera->GetGameObject()->GetTransform()->Rotate(0.0f, -90.0f, 0.0f, false);
}

void SponzaScene::OnUpdate(GameContext* pContext)
{
	Super::OnUpdate(pContext);

	if (GMSandbox::_AutoPanCamera)
	{
		auto pTransform = pContext->m_pActiveCamera->GetGameObject()->GetTransform();
		auto prevPostion = pTransform->GetWorldPosition();
		pTransform->Translate(prevPostion + glm::vec3(0.5f, 0.0f, 0.0f) * pContext->elapsedSec);
	}

	if (GMSandbox::_RunLightCycle)
	{
		m_SunAngle += 5.0f * pContext->elapsedSec;
		if (m_SunAngle > 360.0f) m_SunAngle = 0;

		//x = r * cos(delta_angle)
		//y = r * sin(delta_angle)

		auto glRenderer = pContext->m_pGLRenderer;
		auto prevLightPos = glRenderer->GetDirectionalLightPos();

		float newXCoord = 20.0f * cos(glm::radians(m_SunAngle));
		float newZCoord = 20.0f * sin(glm::radians(m_SunAngle));
		auto newLightPos = glm::vec3(newXCoord, prevLightPos.y, newZCoord);
		glRenderer->SetDirectionalLightPos(newLightPos);
	}
}