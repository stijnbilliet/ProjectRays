#pragma once
#include "Patterns/BaseObject.h"

class SceneManager; class Renderer; class InputManager;
class BasePlayerController; class BasePawn;
class BaseGamemode : public BaseObject
{
public:
	friend class Game;

	BaseGamemode();
	virtual ~BaseGamemode();

	void OnInit() override;
	void OnFixedUpdate(float stepRate) override;
	void OnUpdate(float deltaTime) override;
	void OnDraw(Renderer* pContext) const override;

	//System accessors
	InputManager* GetInputManager() const;
	SceneManager* GetSceneManager() const;

	//Entity accessors
	void AddPlayer(BasePlayerController* pPlyContr);
	BasePlayerController* GetPlayerController(uint8_t plyIndex) const;
	std::vector<BasePlayerController*> GetPlayers() const;
protected:

	void AutoPosses(BasePawn* pPawn);

	InputManager* m_pInputManager;
	SceneManager* m_pSceneManager;

	std::vector<BasePlayerController*> m_pPlayers;
};