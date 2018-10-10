#pragma once
#include "Scenegraph/BasePanel.h"

class GL_Renderer;
class GameObject;
class CameraComponent;
class Scene : public BasePanel
{
public:
	explicit Scene(const std::string& name);
	virtual ~Scene();

	Scene(const Scene& other) = delete;
	Scene(Scene&& other) = delete;
	Scene& operator=(const Scene& other) = delete;
	Scene& operator=(Scene&& other) = delete;

	virtual void OnInit() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnDraw(GL_Renderer* pContext) const override;

	CameraComponent* GetActiveCamera() const;
	void SetActiveCamera(CameraComponent* pNewCamera);
private:
	CameraComponent* m_pActiveCamera;
};