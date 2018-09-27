#pragma once
#include "Scenegraph/SceneManager.h"
#include "Canvas.h"

class UserInterface final : public BasePanelManager<Canvas>
{
public:
	UserInterface();
	virtual ~UserInterface();
};