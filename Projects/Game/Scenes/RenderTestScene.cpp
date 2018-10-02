#include "GamePCH.h"
#include "RenderTestScene.h"
#include "Content/Model/Mesh.h"

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

	//VERT1
	vert.Position = glm::vec3(0.5f, 0.5f, 0.0f);
	vertices.push_back(vert);

	//VERT2
	vert.Position = glm::vec3(0.5f, -0.5f, 0.0f);
	vertices.push_back(vert);

	//VERT3
	vert.Position = glm::vec3(-0.5f, -0.5f, 0.0f);
	vertices.push_back(vert);

	//VERT4
	vert.Position = glm::vec3(-0.5f, 0.5f, 0.0f);
	vertices.push_back(vert);

	std::vector<unsigned int> indices{
		0, 1, 3,
		1, 2, 3
	};

	auto pMesh = new Mesh(vertices, indices);
	auto pMeshDrawComp = new MeshDrawComponent(pMesh);
	pFirstQuad->AddComponent(pMeshDrawComp);
	Add(pFirstQuad);
}