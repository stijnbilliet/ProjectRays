#pragma once
#include "Patterns/SingleInstance.h"

class GL_Renderer;
class CL_Renderer;
class BaseGamemode;
class Application final : public SingleInstance<Application>
{
public:
	Application(BaseGamemode* pGamemode, GL_Renderer* pGLRenderer, CL_Renderer* pCLRenderer, const std::vector<std::string>& args);
	virtual ~Application();

	void Run();
private:
	void OnInit() override;
	void OnUpdate(GameContext* pGameContext) override;

	void Render();
	bool HandleInput();
	void ParseCmdLineArgs();
	void FillPropertyManager();

	GameContext* m_pGameContext;
	std::vector<std::string> m_Args;
};