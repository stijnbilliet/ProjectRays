#include "GamePCH.h"
#include "RenderTestScene.h"

using Super = Scene;

RenderTestScene::RenderTestScene()
	:Scene("RenderTestScene"), m_TotalRotation(0.0f)
{
}

RenderTestScene::~RenderTestScene()
{
	safe_delete(m_pModel);
}

void RenderTestScene::OnUpdate(GameContext* pContext)
{
	Super::OnUpdate(pContext);

	float elapsedSec = pContext->elapsedSec;
	m_TotalRotation += 20.0f*elapsedSec;
	if (m_TotalRotation > 360.0f) m_TotalRotation = 0;

	//m_pTestObject->GetTransform()->Rotate(0.0f, m_TotalRotation, 0.0f, false);
}

void RenderTestScene::PreInit(GameContext* pContext)
{
	Super::PreInit(pContext);

	//Create object
	m_pTestObject = new GameObject();
	Add(m_pTestObject);

	//Load model and fetch meshes
	m_pModel = new Model("Models/Suicune.obj");
	auto pSuicuneMesh = &(m_pModel->GetMeshes()[0]);
	auto pPlatformMesh = &(m_pModel->GetMeshes()[1]);

	std::vector<Texture> textureVect{};
	textureVect.push_back(Texture("Textures/Suicune/SuicuneSaix_A.tga.png", TextureType::DIFFUSE));
	textureVect.push_back(Texture("Textures/Suicune/SuicuneSaix_N.tga.png", TextureType::NORMAL));
	textureVect.push_back(Texture("Textures/Suicune/SuicuneSaix_AO.tga.png", TextureType::AMBIENT));
	pSuicuneMesh->SetTextures(textureVect);

	std::vector<Texture> otherTextureVect{};
	otherTextureVect.push_back(Texture("Textures/spnza_bricks_a_diff.png", TextureType::DIFFUSE));
	pPlatformMesh->SetTextures(otherTextureVect);

	//Load shaders and bundle in shaderprogram
	auto pShaderProgram = new ShaderProgram("Shaders/Deferred_Geometry.vs", "Shaders/Deferred_Geometry.fs");

	//Create draw component
	auto pMeshDrawComp = new MeshDrawComponent(pSuicuneMesh, pShaderProgram);
	m_pTestObject->AddComponent(pMeshDrawComp);

	auto pRayShapeComponent = new RayShapeComponent(pSuicuneMesh);
	m_pTestObject->AddComponent(pRayShapeComponent);

	//Translate object and add to scene
	auto pPlatformObject = new GameObject();
	pMeshDrawComp = new MeshDrawComponent(pPlatformMesh, pShaderProgram);
	pPlatformObject->AddComponent(pMeshDrawComp);

	pRayShapeComponent = new RayShapeComponent(pPlatformMesh);
	pPlatformObject->AddComponent(pRayShapeComponent);
	m_pTestObject->Add(pPlatformObject);
}

void RenderTestScene::PostInit(GameContext * pContext)
{
	Super::PostInit(pContext);

	pContext->m_pActiveCamera->GetGameObject()->GetTransform()->Translate(0.0f, 10.0f, 50.0f);
}
