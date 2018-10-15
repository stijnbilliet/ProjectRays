#include "FrameworkPCH.h"
#include "CameraComponent.h"

CameraComponent::CameraComponent()
	:m_NearPlane(0.1f), m_FarPlane(100.0f), m_FOV(60.0f), m_UsePerspective(true), m_Orthosize(1.0f)
{
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::OnDraw(GameContext* pContext) const
{
	glm::vec3 camPos = m_pAttachedTo->GetTransform()->GetWorldPosition();
	unsigned int shaderId = pContext->m_pGLRenderer->GetLightDrawer()->GetId();

	unsigned int viewPosVar = glGetUniformLocation(shaderId, "viewPos");
	glUniform3f(viewPosVar, camPos.x, camPos.y, camPos.z);
}

void CameraComponent::OnUpdate(GameContext* pContext)
{
	UNREFERENCED_PARAMETER(pContext);

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

	glm::vec3 camPos = m_pAttachedTo->GetTransform()->GetWorldPosition();
	glm::vec3 camForward = m_pAttachedTo->GetTransform()->GetForward();
	glm::vec3 camUp = m_pAttachedTo->GetTransform()->GetUp();

	m_View = glm::lookAt(camPos, camPos + camForward, camUp);
	m_ViewProjection = m_Projection * m_View;
	m_ViewInverse = glm::inverse(m_View);
	m_ProjectionInverse = glm::inverse(m_Projection);
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