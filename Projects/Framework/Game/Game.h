#pragma once
#include "Patterns/SingleInstance.h"

class Renderer;
class BaseGamemode;
class Game final : public SingleInstance<Game>
{
public:
	Game(BaseGamemode* pGamemode, Renderer* pRenderer, const std::vector<std::string>& args);
	virtual ~Game();

	void Run();

private:
	void OnInit() override;
	void OnUpdate(float deltaTime) override;

	void Render();
	bool HandleInput();

	Renderer* m_pRenderer;
	BaseGamemode* m_pGameMode;
	std::vector<std::string> m_Args;
};