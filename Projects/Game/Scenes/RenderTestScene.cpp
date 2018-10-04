#include "GamePCH.h"
#include "RenderTestScene.h"
#include "Content/Model/Mesh.h"
#include "Content/Model/Model.h"
#include "Content/Shader/ShaderProgram.h"
#include "Components/TransformComponent.h"

using Super = Scene;

RenderTestScene::RenderTestScene()
	:Scene("RenderTestScene"), m_TotalRotation(0.0f)
{
}

RenderTestScene::~RenderTestScene()
{
	safe_delete(m_pModel);
}

void RenderTestScene::OnUpdate(float elapsedSec)
{
	Super::OnUpdate(elapsedSec);

	m_TotalRotation += 60.0f*elapsedSec;
	if (m_TotalRotation > 360.0f) m_TotalRotation = 0;

	m_pTestObject->GetTransform()->Rotate(0.0f, m_TotalRotation, 0.0f, false);
}

void RenderTestScene::OnInit()
{
	Super::OnInit();

	//Create object
	m_pTestObject = new GameObject();

	//Load model and fetch meshes
	m_pModel = new Model("Models/LogoBox.fbx");
	auto pMesh = &(m_pModel->GetMeshes()[0]);

	std::vector<Texture> textureVect{};
	textureVect.push_back(Texture("Textures/logo.jpg"));
	pMesh->SetTextures(textureVect);

	//Load shaders and bundle in shaderprogram
	auto pShaderProgram = new ShaderProgram("Shaders/Basic.vs", "Shaders/Basic.fs");

	//Create draw component
	auto pMeshDrawComp = new MeshDrawComponent(pMesh, pShaderProgram);
	m_pTestObject->AddComponent(pMeshDrawComp);

	//Translate object and add to scene
	m_pTestObject->GetTransform()->Translate(0.0f, -4.0f, -15.0f);
	Add(m_pTestObject);
}