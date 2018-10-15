#pragma once
#include "Patterns/BaseObject.h"

class SceneManager; class GL_Renderer; class InputManager;
class GameContext;
class BaseGamemode : public BaseObject
{
public:
	friend class Application;

	BaseGamemode();
	virtual ~BaseGamemode();

	void OnUpdate(GameContext* pContext) override;
	void OnDraw(GameContext* pContext) const override;
};