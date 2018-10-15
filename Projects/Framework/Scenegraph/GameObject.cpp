#include "FrameworkPCH.h"
#include "GameObject.h"
#include "Components/TransformComponent.h"
#include "Scenegraph/BasePanel.h"

using Super = ParentObject<GameObject>;

GameObject::GameObject()
	:ParentObject<GameObject>()
{
	m_pRootComponent = new TransformComponent();
	AddComponent(m_pRootComponent);
}

GameObject::~GameObject()
{
	//Remove yourself because baseclass removes children
	for (auto pComp : m_pComponents)
	{
		safe_delete(pComp);
	}
}

void GameObject::OnInit(GameContext* pGameContext)
{
	Super::OnInit(pGameContext);

	for (auto comp : m_pComponents)
	{
		comp->Init(pGameContext);
	}

	for (auto child : m_pChildren)
	{
		child->Init(pGameContext);
	}
}

void GameObject::OnUpdate(GameContext* pContext)
{
	Super::OnUpdate(pContext);

	for (auto comp : m_pComponents)
	{
		comp->Update(pContext);
	}

	for (auto child : m_pChildren)
	{
		child->Update(pContext);
	}
}

void GameObject::OnDraw(GameContext* pContext) const
{
	Super::OnDraw(pContext);

	for (auto comp : m_pComponents)
	{
		comp->Draw(pContext);
	}

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

std::list<BaseComponent*> GameObject::GetAllComponents() const
{
	return m_pComponents;
}

void GameObject::OnChildAdd(GameObject * pChild)
{
	pChild->SetScene(m_pScene);
}

void GameObject::AddComponent(BaseComponent * pComp)
{
	m_pComponents.push_back(pComp);
	pComp->Attach(this);
}

void GameObject::RemoveComponent(BaseComponent * pComp)
{
	if (pComp)
	{
		auto it = std::find(m_pComponents.begin(), m_pComponents.end(), pComp);
		m_pComponents.remove(*it);
		pComp->Detach();
		safe_delete(pComp);
	}
}

TransformComponent* GameObject::GetTransform() const
{
	return m_pRootComponent;
}