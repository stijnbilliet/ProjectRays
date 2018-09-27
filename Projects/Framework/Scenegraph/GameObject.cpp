#include "FrameworkPCH.h"
#include "GameObject.h"
#include "Components/SceneComponent.h"
#include "Scenegraph/BasePanel.h"
#include "Data/Data.h"

using Super = ParentObject<GameObject>;

GameObject::GameObject()
	:ParentObject<GameObject>()
{
	auto pDefaultRootComp = new SceneComponent();
	SetRoot(pDefaultRootComp);
}

GameObject::~GameObject()
{
	//Remove yourself because baseclass removes children
	for (auto pComp : m_pComponents)
	{
		safe_delete(pComp);
	}
}

void GameObject::OnInit()
{
	Super::OnInit();

	for (auto comp : m_pComponents)
	{
		comp->Init();
	}

	for (auto child : m_pChildren)
	{
		child->Init();
	}
}

void GameObject::OnUpdate(float deltaTime)
{
	Super::OnUpdate(deltaTime);

	for (auto comp : m_pComponents)
	{
		comp->Update(deltaTime);
	}

	for (auto child : m_pChildren)
	{
		child->Update(deltaTime);
	}
}

void GameObject::OnDraw(Renderer* pContext) const
{
	Super::OnDraw(pContext);

	//Tells openGL that we are going to draw at this objects location
	//This results in drawing the gameobjects at their respective world positions
	auto objPos = GetRootComponent()->GetWorldPosition();
	auto objRot = GetRootComponent()->GetWorldRotation();
	auto objScale = GetRootComponent()->GetWorldScale();
	glPushMatrix();
		glTranslatef(objPos.x, objPos.y, 0.0f);
		glRotatef(objRot, 0.0f, 0.0f, 1.0f);
		glScalef(objScale.x, objScale.y, 1.0f);

		for (auto comp : m_pComponents)
		{
			comp->Draw(pContext);
		}
	glPopMatrix();

	for (auto child : m_pChildren)
	{
		child->Draw(pContext);
	}
}

BasePanel* GameObject::GetScene()
{
	return m_pScene;
}

void GameObject::SetScene(BasePanel * pScene)
{
	m_pScene = pScene;
}

std::list<ObjectComponent*> GameObject::GetAllComponents() const
{
	return m_pComponents;
}

void GameObject::OnChildAdd(GameObject * pChild)
{
	pChild->SetScene(m_pScene);
}

void GameObject::AddComponent(ObjectComponent * pComp)
{
	m_pComponents.push_back(pComp);
	pComp->Attach(this);
}

void GameObject::RemoveComponent(ObjectComponent * pComp)
{
	if (pComp)
	{
		auto it = std::find(m_pComponents.begin(), m_pComponents.end(), pComp);
		m_pComponents.remove(*it);
		pComp->Detach();
		safe_delete(pComp);
	}
}

void GameObject::SetRoot(SceneComponent * pNewRoot)
{
	AddComponent(pNewRoot);
	RemoveComponent(m_pRootComponent);
	m_pRootComponent = pNewRoot;
}

SceneComponent* GameObject::GetRootComponent() const
{
	return m_pRootComponent;
}