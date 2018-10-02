#include "FrameworkPCH.h"
#include "CameraComponent.h"
#include "PropertyManager.h"
#include "TransformComponent.h"

CameraComponent::CameraComponent()
	:m_NearPlane(0.1f), m_FarPlane(100.0f), m_FOV(60.0f)
{
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::OnUpdate(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);

	int width{};
	int height{};
	float aspectratio{};
	
	PropertyManager::GetInstance().GetInt("renderer_viewwidth", width);
	PropertyManager::GetInstance().GetInt("renderer_viewheight", height);
	aspectratio = (float)width / height;

	if (m_UsePerspective)
	{
		m_Projection = glm::perspective(glm::radians(m_FOV), aspectratio, m_NearPlane, m_FarPlane);
	}
	else
	{
		width = (m_Orthosize>0) ? int(m_Orthosize * aspectratio) : width;
		height = (m_Orthosize>0) ? int(m_Orthosize) : height;
		m_Projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height, m_NearPlane, m_FarPlane);
	}

	glm::vec3 worldPos = m_pAttachedTo->GetTransform()->GetWorldPosition();
	glm::vec3 lookAt = m_pAttachedTo->GetTransform()->GetForward();
	glm::vec3 upVec = m_pAttachedTo->GetTransform()->GetUp();

	m_View = glm::lookAt(worldPos, worldPos+ lookAt, upVec);
	m_ViewProjection = m_View * m_Projection;
	m_ViewInverse = glm::inverse(m_View);
	m_ProjectionInverse = glm::inverse(m_View);
	m_ViewProjectionInverse = glm::inverse(m_ViewProjection);
}

const glm::mat4 & CameraComponent::GetView() const
{
	return m_View;
}

const glm::mat4 & CameraComponent::GetProjection() const
{
	return m_Projection;
}

const glm::mat4 & CameraComponent::GetProjectionInverse() const
{
	return m_ProjectionInverse;
}

const glm::mat4 & CameraComponent::GetViewProjection() const
{
	return m_ViewProjection;
}

const glm::mat4 & CameraComponent::GetViewInverse() const
{
	return m_ViewInverse;
}

const glm::mat4 & CameraComponent::GetViewProjectionInverse() const
{
	return m_ViewProjectionInverse;
}