#include "GamePCH.h"
#include "RenderTestScene.h"
#include "Content/Model/Mesh.h"
#include "Content/Shader/ShaderProgram.h"
#include "Components/TransformComponent.h"

using Super = Scene;

RenderTestScene::RenderTestScene()
	:Scene("RenderTestScene"), m_TotalRotation(0.0f)
{
}

RenderTestScene::~RenderTestScene()
{
}

void RenderTestScene::OnUpdate(float elapsedSec)
{
	Super::OnUpdate(elapsedSec);

	m_TotalRotation += 60.0f*elapsedSec;
	if (m_TotalRotation > 360.0f) m_TotalRotation = 0;

	m_pFirstQuad->GetTransform()->Rotate(m_TotalRotation, m_TotalRotation, m_TotalRotation, false);
}

void RenderTestScene::OnInit()
{
	Super::OnInit();

	m_pFirstQuad = new GameObject();

	std::vector<Vertex> verticesvector{};

	//TODO: FOR THE TIME BEING MANUAL

	std::vector<Vertex> verticesVect{};

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};

	for (int i = 0; i < 108; i += 3)
	{
		auto newVert = Vertex(vertices[i], vertices[i + 1], vertices[i + 2]);
		verticesVect.push_back(newVert);
	}

	auto pMesh = new Mesh(verticesVect);

	//LOAD SHADER
	//load shader from file
	std::string assetPath{};
	PropertyManager::GetInstance().GetString("assetpath", assetPath);

	std::string vshPathStr{ assetPath + "/Shaders/BasicShading.vsh" };
	std::string fshPathStr{ assetPath + "/Shaders/BasicShading.fsh" };
	auto pShaderProgram = new ShaderProgram(vshPathStr.c_str(), fshPathStr.c_str());

	//CREATE DRAW COMPONENT
	auto pMeshDrawComp = new MeshDrawComponent(pMesh, pShaderProgram);
	m_pFirstQuad->AddComponent(pMeshDrawComp);

	m_pFirstQuad->GetTransform()->Translate(0.0f, 0.0f, 0.0f);
	Add(m_pFirstQuad);
}