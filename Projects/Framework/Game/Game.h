#pragma once
#include "Patterns/SingleInstance.h"

class GL_Renderer;
class CL_Renderer;
class BaseGamemode;
class Game final : public SingleInstance<Game>
{
public:
	Game(BaseGamemode* pGamemode, GL_Renderer* pRenderer, const std::vector<std::string>& args);
	virtual ~Game();

	void Run();
private:
	void OnInit() override;
	void OnUpdate(float deltaTime) override;

	void Render();
	bool HandleInput();
	void ParseCmdLineArgs();
	void FillPropertyManager();

	GL_Renderer* m_pRenderer;
	CL_Renderer* m_pRaytracer;

	BaseGamemode* m_pGameMode;
	std::vector<std::string> m_Args;
};