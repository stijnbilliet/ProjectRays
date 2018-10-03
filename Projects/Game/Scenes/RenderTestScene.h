#pragma once
#include "Scenegraph/Scene.h"

class RenderTestScene : public Scene
{
public:
	RenderTestScene();
	virtual ~RenderTestScene();

	virtual void OnUpdate(float elapsedSec) override;
	virtual void OnInit() override;
private:
	GameObject * m_pFirstQuad;
	float m_TotalRotation;
};