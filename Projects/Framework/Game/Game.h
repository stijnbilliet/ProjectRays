#pragma once
#include "Patterns/SingleInstance.h"

class Renderer;
class BaseGamemode;
class Game final : public SingleInstance<Game>
{
public:
	Game();
	~Game();


	void SetGameMode(BaseGamemode* gm);
	void Run();

private:
	void OnInit() override;
	void OnFixedUpdate(float stepRate) override;
	void OnUpdate(float deltaTime) override;

	void Render();
	void UpdateGame(float deltaTime);
	void Integrate(float deltaTime);

	void Clean();
	bool HandleInput();

	Renderer* m_pRenderer;
	BaseGamemode* m_pGameMode;

	float m_Accumulator;
	float m_StepRate;

	std::mutex m_PhysicsMutex;
};