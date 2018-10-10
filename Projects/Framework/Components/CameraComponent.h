#pragma once
#include "Components/BaseComponent.h"

class CameraComponent : public BaseComponent
{
public:
	CameraComponent();
	virtual ~CameraComponent();

	virtual void OnDraw(GL_Renderer* pContext) const override;
	virtual void OnUpdate(float elapsedSec) override;

	const glm::mat4& GetView() const;
	const glm::mat4& GetProjection() const;
	const glm::mat4& GetProjectionInverse() const;
	const glm::mat4& GetViewProjection() const;
	const glm::mat4& GetViewInverse() const;
	const glm::mat4& GetViewProjectionInverse() const;

private:
	float m_FOV;
	float m_Orthosize;
	float m_NearPlane;
	float m_FarPlane;

	bool m_UsePerspective;

	glm::mat4 m_View, m_Projection, m_ViewInverse, m_ViewProjection, m_ViewProjectionInverse, m_ProjectionInverse;
};