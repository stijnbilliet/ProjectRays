#pragma once
#include "Scenegraph/BasePanel.h"

class Renderer;
class GameObject;
class BaseCamera;
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
	virtual void OnDraw(Renderer* pContext) const override;

	BaseCamera* GetCamera();
protected:
	BaseCamera* m_pCamera;
};