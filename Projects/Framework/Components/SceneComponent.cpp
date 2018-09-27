#include "FrameworkPCH.h"
#include "SceneComponent.h"
#include "Scenegraph/GameObject.h"

using Super = ObjectComponent;

SceneComponent::SceneComponent()
	:ObjectComponent(), ParentObject<SceneComponent>(), m_Position(), m_Rotation(), m_WorldRotation(), m_Scale(1.0f, 1.0f), m_WorldScale(1.0f, 1.0f), m_bIsDirty(true), m_bWasTranslated(false)
{
}

SceneComponent::~SceneComponent()
{
}

void SceneComponent::Translate(const glm::vec2& pos)
{
	m_Position.x = pos.x;
	m_Position.y = pos.y;
	m_bIsDirty = true;
}

void SceneComponent::Rotate(const float newRot)
{
	m_Rotation = newRot;
	m_bIsDirty = true;
}

void SceneComponent::Scale(const glm::vec2& scale)
{
	m_Scale.x = scale.x;
	m_Scale.y = scale.y;
	m_bIsDirty = true;
}

const glm::mat4x4 & SceneComponent::GetWorldMatrix() const
{
	return m_World;
}

const glm::vec2& SceneComponent::GetLocalPosition() const
{
	return m_Position;
}

const glm::vec2 & SceneComponent::GetWorldPosition() const
{
	return m_WorldPosition;
}

const float SceneComponent::GetLocalRotation() const
{
	return m_Rotation;
}

const float SceneComponent::GetWorldRotation() const
{
	return m_WorldRotation;
}

const glm::vec2& SceneComponent::GetLocalScale() const
{
	return m_Scale;
}

const glm::vec2 & SceneComponent::GetWorldScale() const
{
	return m_WorldScale;
}

void SceneComponent::SetWorldPosition(const glm::vec2& pos)
{
	m_WorldPosition = pos;
}

void SceneComponent::SetWorldRotation(const float rot)
{
	m_WorldRotation = rot;
}

void SceneComponent::SetWorldScale(const glm::vec2& scale)
{
	m_WorldScale = scale;
}

bool SceneComponent::WasTranslated() const
{
	return m_bWasTranslated;
}

void SceneComponent::OnUpdate(float elapsedSec)
{
	m_bWasTranslated = false;

	//Update yourself
	if (m_bIsDirty)
	{
		RebuildWorldMatrix();
		m_bIsDirty = false;
		m_bWasTranslated = true;
	}

	//Update your children
	for (auto child : m_pChildren)
	{
		dynamic_cast<ObjectComponent*>(child)->Update(elapsedSec);
	}
}

void SceneComponent::OnDraw(Renderer* pContext) const
{
	//Draw your children, using the same drawing techniques as for the gameobject
	for (auto child : m_pChildren)
	{
		auto compPos = child->GetLocalPosition();
		auto compRot = child->GetLocalRotation();
		auto compScale = child->GetLocalScale();
		glPushMatrix();
			glTranslatef(compPos.x, compPos.y, 0.0f);
			glRotatef(compRot, 0.0f, 0.0f, 1.0f);
			glScalef(compScale.x, compScale.y, 1.0f);

			dynamic_cast<ObjectComponent*>(child)->Draw(pContext);
		glPopMatrix();
	}
}

void SceneComponent::RebuildWorldMatrix()
{
	//Build our own world matrix
	m_World = glm::scale(glm::mat4x4(), glm::vec3(m_Scale.x, m_Scale.y, 1.0f))
		*glm::rotate(glm::mat4x4(), m_Rotation, glm::vec3(0.0f, 0.0f, 1.0f))
		*glm::translate(glm::mat4x4(), glm::vec3(m_Position.x, m_Position.y, 0.0f));

	//Should perform scenegraph?
	auto pParentComponent = GetParent();
	if (pParentComponent)
	{
		m_World *= pParentComponent->GetWorldMatrix();
	}
	else if (m_pAttachedTo && m_pAttachedTo->GetParent())
	{
		m_World *= m_pAttachedTo->GetParent()->GetRootComponent()->GetWorldMatrix();
	}

	//Get transform from updated world matrix
	//*New Position
	m_WorldPosition.x = m_World[3][0];
	m_WorldPosition.y = m_World[3][1];
}