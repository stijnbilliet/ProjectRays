#pragma once
#include "Scenegraph/BasePanelManager.h"
#include "Scenegraph/Scene.h"

class GL_Renderer;
class SceneManager final : public BasePanelManager<Scene>
{
public:
	SceneManager();
	virtual ~SceneManager();
};