#include "FrameworkPCH.h"
#include "Scene.h"
#include "GameObject.h"

#include "Components/CameraComponent.h"

Scene::Scene(const std::string& name)
	:BasePanel(name)
{
}

Scene::~Scene()
{
	safe_delete(m_pActiveCamera);
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

CameraComponent * Scene::GetActiveCamera() const
{
	return nullptr;
}

void Scene::SetActiveCamera(CameraComponent * pNewCamera)
{
	UNREFERENCED_PARAMETER(pNewCamera);
	//TODO: create default camera either in scene or anywhere else
}
