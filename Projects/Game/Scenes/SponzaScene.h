#pragma once
#include "Scenegraph/Scene.h"

class SponzaScene : public Scene
{
public:
	SponzaScene();
	virtual ~SponzaScene();
private:
	virtual void PreInit(GameContext* pContext) override;
	virtual void PostInit(GameContext* pContext) override;
	virtual void OnUpdate(GameContext* pContext) override;

	std::vector<Mesh> m_Meshes;
	float m_SunAngle;
};