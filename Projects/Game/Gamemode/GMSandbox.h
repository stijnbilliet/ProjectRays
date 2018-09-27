#pragma once
#include "Game/BaseGamemode.h"

class Renderer;
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
	void PostInit() override;
private:
	void InitInput(unsigned int playerIndex);

	Scene* m_pCornelScene;
};