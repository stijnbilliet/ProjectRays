#pragma once
#include "Scenegraph/Scene.h"

class Model;
class RenderTestScene : public Scene
{
public:
	RenderTestScene();
	virtual ~RenderTestScene();

	virtual void OnUpdate(GameContext* pContext) override;
	virtual void PreInit(GameContext* pContext) override;
	virtual void PostInit(GameContext* pContext) override;
private:
	GameObject * m_pTestObject;
	Model* m_pModel;
	Mesh* m_pMesh;
	float m_TotalRotation;
};