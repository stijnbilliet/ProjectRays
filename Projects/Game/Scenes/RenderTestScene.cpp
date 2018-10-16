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

	m_pTestObject->GetTransform()->Rotate(0.0f, m_TotalRotation, 0.0f, false);
}

void RenderTestScene::PreInit(GameContext* pContext)
{
	Super::OnInit(pContext);

	//Create object
	m_pTestObject = new GameObject();

	//Load model and fetch meshes
	m_pModel = new Model("Models/SuicuneSAIX.fbx");
	auto pMesh = &(m_pModel->GetMeshes()[0]);

	std::vector<Texture> textureVect{};
	textureVect.push_back(Texture("Textures/Suicune/SuicuneSaix_A.tga.png", TextureType::DIFFUSE));
	textureVect.push_back(Texture("Textures/Suicune/SuicuneSaix_N.tga.png", TextureType::NORMAL));
	textureVect.push_back(Texture("Textures/Suicune/SuicuneSaix_AO.tga.png", TextureType::AMBIENT));
	pMesh->SetTextures(textureVect);

	//Load shaders and bundle in shaderprogram
	auto pShaderProgram = new ShaderProgram("Shaders/Deferred_Geometry.vs", "Shaders/Deferred_Geometry.fs");

	//Create draw component
	auto pMeshDrawComp = new MeshDrawComponent(pMesh, pShaderProgram);
	m_pTestObject->AddComponent(pMeshDrawComp);

	auto pRayShapeComponent = new RayShapeComponent(pMesh);
	m_pTestObject->AddComponent(pRayShapeComponent);

	//Translate object and add to scene
	m_pTestObject->GetTransform()->Translate(0.0f, -7.5f, -25.0f);
	m_pTestObject->GetTransform()->Scale(1/20.0f, 1/20.0f, 1/ 20.0f);

	Add(m_pTestObject);
}