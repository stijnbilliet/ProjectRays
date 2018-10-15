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

	virtual void OnInit(GameContext* pGameContext) override;
	virtual void OnUpdate(GameContext* pContext) override;
	virtual void OnDraw(GameContext* pContext) const override;

	CameraComponent* GetActiveCamera() const;
	void SetActiveCamera(CameraComponent* pNewCamera);
private:
	CameraComponent* m_pActiveCamera;
};