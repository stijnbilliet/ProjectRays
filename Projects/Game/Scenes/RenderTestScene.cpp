#include "GamePCH.h"
#include "RenderTestScene.h"
#include "Content/Model/Mesh.h"
#include "Content/Shader/ShaderProgram.h"

using Super = Scene;

RenderTestScene::RenderTestScene()
	:Scene("RenderTestScene")
{
}

RenderTestScene::~RenderTestScene()
{
}

void RenderTestScene::OnUpdate(float elapsedSec)
{
	Super::OnUpdate(elapsedSec);
}

void RenderTestScene::OnInit()
{
	Super::OnInit();

	auto pFirstQuad = new GameObject();

	//TODO: FOR THE TIME BEING MANUAL
	std::vector<Vertex> vertices{};
	auto vert = Vertex();

	//VERT0
	vert.Position = glm::vec3(-0.5f, -0.5f, 0.0f);
	vertices.push_back(vert);

	//VERT1
	vert.Position = glm::vec3(0.5f, -0.5f, 0.0f);
	vertices.push_back(vert);

	//VERT2
	vert.Position = glm::vec3(0.0f, 0.5f, 0.0f);
	vertices.push_back(vert);

	std::vector<unsigned int> indices{
		0, 1, 2,
	};

	auto pMesh = new Mesh(vertices, indices);

	//LOAD SHADER
	//load shader from file
	std::string assetPath{};
	PropertyManager::GetInstance().GetString("assetpath", assetPath);

	std::string vshPathStr{ assetPath + "/Shaders/BasicShading.vsh" };
	std::string fshPathStr{ assetPath + "/Shaders/BasicShading.fsh" };
	auto pShaderProgram = new ShaderProgram(vshPathStr.c_str(), fshPathStr.c_str());

	//CREATE DRAW COMPONENT
	auto pMeshDrawComp = new MeshDrawComponent(pMesh, pShaderProgram);
	pFirstQuad->AddComponent(pMeshDrawComp);
	Add(pFirstQuad);
}