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
	m_pModel = new Model("Models/Suicune/Suicune.obj");
	auto pSuicuneMesh = &(m_pModel->GetMeshes()[0]);

	std::vector<Texture> textureVect{};
	textureVect.push_back(Texture("Textures/Suicune/SuicuneSaix_A.tga.png", TextureType::DIFFUSE));
	textureVect.push_back(Texture("Textures/Suicune/SuicuneSaix_N.tga.png", TextureType::NORMAL));
	textureVect.push_back(Texture("Textures/Suicune/SuicuneSaix_M.tga.png", TextureType::SPECULAR));
	pSuicuneMesh->SetTextures(textureVect);

	//Load shaders and bundle in shaderprogram
	auto pShaderProgram = new ShaderProgram("Shaders/Deferred_Geometry.vs", "Shaders/Deferred_Geometry.fs");

	//Create draw component
	auto pMeshDrawComp = new MeshDrawComponent(pSuicuneMesh, pShaderProgram);
	m_pTestObject->AddComponent(pMeshDrawComp);

	auto pRayShapeComponent = new RayShapeComponent(pSuicuneMesh);
	m_pTestObject->AddComponent(pRayShapeComponent);
}

void RenderTestScene::PostInit(GameContext * pContext)
{
	Super::PostInit(pContext);

	pContext->m_pActiveCamera->GetGameObject()->GetTransform()->Translate(0.0f, 10.0f, 35.0f);
}
