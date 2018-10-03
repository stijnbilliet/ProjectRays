#include "FrameworkPCH.h"
#include "Scene.h"
#include "GameObject.h"

#include "Components/CameraComponent.h"
#include "Components/TransformComponent.h"

Scene::Scene(const std::string& name)
	:BasePanel(name)
{
	//TODO: not ideal solution
	auto pCamera = new GameObject();
	m_pActiveCamera = new CameraComponent();

	pCamera->AddComponent(m_pActiveCamera);
	pCamera->GetTransform()->Translate(0.0f, 0.0f, 3.0f);
	Add(pCamera);
}

Scene::~Scene()
{
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

CameraComponent* Scene::GetActiveCamera() const
{
	return m_pActiveCamera;
}

void Scene::SetActiveCamera(CameraComponent * pNewCamera)
{
	m_pActiveCamera = pNewCamera;
}
