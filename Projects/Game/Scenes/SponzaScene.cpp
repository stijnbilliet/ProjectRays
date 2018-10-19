#include "GamePCH.h"
#include "SponzaScene.h"

using Super = Scene;

SponzaScene::SponzaScene()
	:Scene("SponzaScene")
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

		auto pMesh = &m_Meshes[i];
		//pMesh->SetTextures();

		auto pMeshDrawComponent = new MeshDrawComponent(pMesh, pShaderProgram);
		auto pRaytraceComponent = new RayShapeComponent(pMesh);
		pChildObj->GetTransform()->Scale(1/80.0f, 1/ 80.0f, 1/ 80.0f);
		pChildObj->AddComponent(pMeshDrawComponent);
		pChildObj->AddComponent(pRaytraceComponent);
	}
}