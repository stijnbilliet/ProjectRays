#pragma once
#include "Game/BaseGamemode.h"

class GL_Renderer;
class BasePawn;
class GameObject;
class Scene;
class TextLabel;
class BasePlayerController;
class GMSandbox : public BaseGamemode
{
public:
	GMSandbox();
	virtual ~GMSandbox();

	void OnUpdate(float deltaTime) override;
	void PreInit() override;

	void ProcessInput();
private:
};