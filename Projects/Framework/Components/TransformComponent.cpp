#include "FrameworkPCH.h"
#include "TransformComponent.h"
#include "Scenegraph/GameObject.h"

using Super = BaseComponent;

TransformComponent::TransformComponent()
	:BaseComponent(),
	m_Position(0.0f, 0.0f, 0.0f), m_Rotation(0.0f, 0.0f, 0.0f, 1.0f), m_Scale(1.0f, 1.0f, 1.0f),
	m_WorldPosition(0.0f, 0.0f, 0.0f), m_WorldRotation(0.0f, 0.0f, 0.0f, 1.0f), m_WorldScale(1.0f, 1.0f, 1.0f),
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

void TransformComponent::Rotate(const glm::vec3& rotVec, bool bEuler)
{
	glm::quat rotQuat;

	if (bEuler)
	{
		//conversion from euler to quat
		rotQuat = glm::quat(rotVec);
	}
	else
	{
		//conversion from radians to quat
		rotQuat = glm::quat(glm::vec3(
			glm::degrees(rotVec.x),
			glm::degrees(rotVec.y),
			glm::degrees(rotVec.z)
		));
	}

	Rotate(rotQuat);
}

void TransformComponent::Rotate(const float x, const float y, const float z, bool bEuler)
{
	auto rotVec = glm::vec3(x, y, z);
	Rotate(rotVec, bEuler);
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

const glm::mat4x4 & TransformComponent::GetWorldMatrix() const
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
	m_World = glm::scale(glm::mat4x4(), glm::vec3(m_Scale.x, m_Scale.y, m_Scale.z))
		*glm::toMat4(glm::quat(glm::vec3(m_Rotation.x, m_Rotation.y, m_Rotation.z)))
		*glm::translate(glm::mat4x4(), glm::vec3(m_Position.x, m_Position.y, m_Position.z));

	//Should perform scenegraph?
	auto parent = m_pAttachedTo->GetParent();
	if (parent)
	{
		m_World *= parent->GetTransform()->GetWorldMatrix();
	}

	//Get transform from updated world matrix (decompose)
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(m_World, m_WorldScale, m_WorldRotation, m_WorldPosition, skew, perspective);

	//decompose returns matrix conjugate
	m_WorldRotation = glm::conjugate(m_WorldRotation);

	//get forward, up and right from worldrotation
	glm::mat4 rotMat = glm::toMat4(m_WorldRotation);
	m_Forward = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f) * rotMat;
	m_Right = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f) * rotMat;
	m_Up = glm::cross(m_Forward, m_Right);
}