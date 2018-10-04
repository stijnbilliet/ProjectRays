#include "FrameworkPCH.h"
#include "TransformComponent.h"
#include "Scenegraph/GameObject.h"

using Super = BaseComponent;

TransformComponent::TransformComponent()
	:BaseComponent(),
	m_Position(0.0f, 0.0f, 0.0f), m_Rotation(glm::vec3(0.0f, 0.0f, 0.0f)), m_Scale(1.0f, 1.0f, 1.0f),
	m_WorldPosition(0.0f, 0.0f, 0.0f), m_WorldRotation(glm::vec3(0.0f, 0.0f, 0.0f)), m_WorldScale(1.0f, 1.0f, 1.0f),
	m_bIsDirty(true), m_bWasTranslated(false)
{
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::Translate(const glm::vec3& pos)
{
	m_Position.x = pos.x;
	m_Position.y = pos.y;
	m_Position.z = pos.z;

	m_bIsDirty = true;
}

void TransformComponent::Translate(const float x, const float y, const float z)
{
	auto transVec = glm::vec3(x, y, z);
	Translate(transVec);
}

void TransformComponent::Rotate(const glm::quat& rotQuat)
{
	m_Rotation = rotQuat;
	m_bIsDirty = true;
}

void TransformComponent::Rotate(const glm::vec3& rotVec, bool bRadians)
{
	glm::quat rotQuat;

	if (bRadians)
	{
		//conversion from euler to quat
		rotQuat = glm::quat(rotVec);
	}
	else
	{
		//conversion from radians to quat
		rotQuat = glm::quat(glm::vec3(
			glm::radians(rotVec.x),
			glm::radians(rotVec.y),
			glm::radians(rotVec.z)
		));
	}

	Rotate(rotQuat);
}

void TransformComponent::Rotate(const float x, const float y, const float z, bool bRadians)
{
	auto rotVec = glm::vec3(x, y, z);
	Rotate(rotVec, bRadians);
}

void TransformComponent::Scale(const glm::vec3& scale)
{
	m_Scale.x = scale.x;
	m_Scale.y = scale.y;
	m_Scale.z = scale.z;

	m_bIsDirty = true;
}

void TransformComponent::Scale(const float x, const float y, const float z)
{
	auto scaleVec = glm::vec3(x, y, z);
	Scale(scaleVec);
}

const glm::mat4 & TransformComponent::GetWorldMatrix() const
{
	return m_World;
}

const glm::vec3& TransformComponent::GetLocalPosition() const
{
	return m_Position;
}

const glm::vec3 & TransformComponent::GetWorldPosition() const
{
	return m_WorldPosition;
}

const glm::quat& TransformComponent::GetLocalRotation() const
{
	return m_Rotation;
}

const glm::quat& TransformComponent::GetWorldRotation() const
{
	return m_WorldRotation;
}

const glm::vec3& TransformComponent::GetLocalScale() const
{
	return m_Scale;
}

const glm::vec3& TransformComponent::GetWorldScale() const
{
	return m_WorldScale;
}

const glm::vec3 & TransformComponent::GetForward() const
{
	return m_Forward;
}

const glm::vec3 & TransformComponent::GetUp() const
{
	return m_Up;
}

const glm::vec3 & TransformComponent::GetRight() const
{
	return m_Right;
}

bool TransformComponent::WasTranslated() const
{
	return m_bWasTranslated;
}

void TransformComponent::OnUpdate(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);

	m_bWasTranslated = false;

	//Update yourself
	if (m_bIsDirty)
	{
		RebuildWorldMatrix();

		m_bIsDirty = false;
		m_bWasTranslated = true;
	}
}

void TransformComponent::RebuildWorldMatrix()
{
	//Build our own world matrix
	//Note on rotation matrix: glm expects euler, from euler to quat and then from quat to rotation matrix
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(), glm::vec3(m_Scale.x, m_Scale.y, m_Scale.z));
	glm::mat4 rotationMatrix = glm::mat4_cast(m_Rotation);
	glm::mat4 translationMatrix = glm::translate(glm::mat4(), glm::vec3(m_Position.x, m_Position.y, m_Position.z));

	m_World = translationMatrix * rotationMatrix * scaleMatrix;

	//Should perform scenegraph?
	auto parent = m_pAttachedTo->GetParent();
	if (parent)
	{
		m_World *= parent->GetTransform()->GetWorldMatrix();
	}

	//Get transform from updated world matrix (decompose)
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::quat rotation;
	glm::decompose(m_World, m_WorldScale, rotation, m_WorldPosition, skew, perspective);

	//decompose returns matrix conjugate
	m_WorldRotation = glm::conjugate(rotation);

	//get forward, up and right from complete worldmatrix
	rotationMatrix = glm::mat4_cast(m_WorldRotation);

	m_Forward = rotationMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
	m_Right = rotationMatrix * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	m_Up = rotationMatrix * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
}