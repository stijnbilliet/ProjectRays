#pragma once
#include "Scenegraph/BasePanelManager.h"
#include "Scenegraph/Scene.h"

class Renderer;
class SceneManager final : public BasePanelManager<Scene>
{
public:
	SceneManager();
	virtual ~SceneManager();

	void OnFixedUpdate(float stepRate) override;
};