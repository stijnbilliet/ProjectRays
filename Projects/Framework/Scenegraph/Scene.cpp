#include "FrameworkPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include "Camera/BaseCamera.h"

Scene::Scene(const std::string& name)
	:BasePanel(name), m_pCamera(nullptr)
{
	m_pCamera = new BaseCamera();
	Add(m_pCamera);
}

Scene::~Scene()
{
}

BaseCamera * Scene::GetCamera()
{
	return m_pCamera;
}

void Scene::OnInit()
{
	for (auto gameObject : m_pObjects)
	{
		gameObject->Init();
	}
}

void Scene::OnUpdate(float deltaTime)
{
	//Update gameObjects
	for (auto gameObject : m_pObjects)
	{
		gameObject->Update(deltaTime);
	}
}

void Scene::OnDraw(Renderer* pContext) const
{
	UNREFERENCED_PARAMETER(pContext);

	for (const auto gameObject : m_pObjects)
	{
		gameObject->Draw(pContext);
	}
}