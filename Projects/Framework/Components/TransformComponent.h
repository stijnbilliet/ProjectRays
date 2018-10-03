#pragma once
#include "Components\BaseComponent.h"

class TransformComponent : public BaseComponent
{
public:
	TransformComponent();
	virtual ~TransformComponent();

	TransformComponent(const TransformComponent& other) = delete;
	TransformComponent(TransformComponent&& other) = delete;
	TransformComponent& operator=(const TransformComponent& other) = delete;
	TransformComponent& operator=(TransformComponent&& other) = delete;

	//BaseObject implementation
	virtual void OnUpdate(float elapsedSec) override;

	//Translation accessors
	void Translate(const glm::vec3& pos);
	void Translate(const float x, const float y, const float z);
	void Rotate(const glm::quat& rotQuat);
	void Rotate(const glm::vec3& rotVec, bool bRadians = false);
	void Rotate(const float x, const float y, const float z, bool bRadians);
	void Scale(const glm::vec3& scale);
	void Scale(const float x, const float y, const float z);

	const glm::mat4& GetWorldMatrix() const;
	const glm::vec3& GetLocalPosition() const;
	const glm::vec3& GetWorldPosition() const;
	const glm::quat& GetLocalRotation() const;
	const glm::quat& GetWorldRotation() const;
	const glm::vec3& GetLocalScale() const;
	const glm::vec3& GetWorldScale() const;

	const glm::vec3& GetForward() const;
	const glm::vec3& GetUp() const;
	const glm::vec3& GetRight() const;

	bool WasTranslated() const;
private:
	void RebuildWorldMatrix();

	glm::vec3 m_Position;
	glm::vec3 m_WorldPosition;
	glm::quat m_Rotation;
	glm::quat m_WorldRotation;
	glm::vec3 m_Scale;
	glm::vec3 m_WorldScale;
	glm::mat4 m_World;

	glm::vec3 m_Forward;
	glm::vec3 m_Up;
	glm::vec3 m_Right;

	bool m_bWasTranslated;
	bool m_bIsDirty;
};