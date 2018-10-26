#include "GamePCH.h"
#include "ShadowTestScene.h"

using Super = Scene;

ShadowTestScene::ShadowTestScene()
	:Scene("ShadowTestScene")
{
}

ShadowTestScene::~ShadowTestScene()
{
	safe_delete(m_pModel);
}

void ShadowTestScene::PreInit(GameContext * pContext)
{
	Super::PreInit(pContext);

	auto pSceneObj = new GameObject();
	Add(pSceneObj);

	auto pShaderProgram = new ShaderProgram("Shaders/Deferred_Geometry.vs", "Shaders/Deferred_Geometry.fs");
	m_pModel = new Model("Models/shadowtestscene.obj");

	std::vector<Texture> textures{};
	textures.push_back(Texture("Textures/spnza_bricks_a_diff.png", TextureType::DIFFUSE));

	for (Mesh& mesh : m_pModel->GetMeshes())
	{

		mesh.SetTextures(textures);
		auto pChildObj = new GameObject();
		pSceneObj->Add(pChildObj);

		auto pMeshDrawComponent = new MeshDrawComponent(&mesh, pShaderProgram);
		auto pRaytraceComponent = new RayShapeComponent(&mesh);
		pChildObj->AddComponent(pMeshDrawComponent);
		pChildObj->AddComponent(pRaytraceComponent);
	}
}

void ShadowTestScene::PostInit(GameContext * pContext)
{
	Super::PostInit(pContext);
	pContext->m_pActiveCamera->GetGameObject()->GetTransform()->Translate(10.0f, 10.0f, 20.0f);
}