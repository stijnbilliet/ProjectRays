#include "FrameworkPCH.h"
#include "Application.h"
#include "Graphics/Renderer/CL_Renderer.h"

Application::Application(BaseGamemode* pGamemode, GL_Renderer* pGLRenderer, CL_Renderer* pCLRenderer, const std::vector<std::string>& args)
	:m_pGameContext(new GameContext()), m_Args(args)
{
	m_pGameContext->m_pGamemode = pGamemode;
	m_pGameContext->m_pGLRenderer = pGLRenderer;
	m_pGameContext->m_pCLRenderer = pCLRenderer;
	m_pGameContext->m_pInputManager = new InputManager();
	m_pGameContext->m_pSceneManager = new SceneManager();
	m_pGameContext->m_pLightManager = new LightManager();

	ParseCmdLineArgs();
	FillPropertyManager();
}

Application::~Application()
{
	safe_delete(m_pGameContext);
}

void Application::Run()
{
	Init(m_pGameContext);

	bool doContinue = true;
	auto lastTime = std::chrono::high_resolution_clock::now();
	while (doContinue)
	{
		auto currentTime = std::chrono::high_resolution_clock::now();
		float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
		lastTime = currentTime;

		//Input
		doContinue = HandleInput();

		//Game Logic
		m_pGameContext->elapsedSec = deltaTime;
		Update(m_pGameContext);

		//Render
		Render();
	}
} 

void Application::ParseCmdLineArgs()
{
	//Store asset path
	std::string exePath = m_Args[0];
	exePath = exePath.substr(0, exePath.find_last_of('\\') + 1);
	std::replace(exePath.begin(), exePath.end(), '\\', '/');
	std::string assetPath = exePath + "../../../Assets";
	PropertyManager::GetInstance().Add("assetpath", assetPath);

	//Parse rest
	//...
}

void Application::FillPropertyManager()
{
	PropertyManager::GetInstance().Add("renderer_viewwidth", "1280");
	PropertyManager::GetInstance().Add("renderer_viewheight", "720");
	PropertyManager::GetInstance().Add("windowtitle", "ProjectRays(OpenGL+RadeonRays)");
}

void Application::OnInit(GameContext* pGameContext)
{
	m_pGameContext->m_pGLRenderer->Init(pGameContext);
	m_pGameContext->m_pGamemode->Init(pGameContext);
	m_pGameContext->m_pInputManager->Init(pGameContext);
	m_pGameContext->m_pSceneManager->Init(pGameContext);
	m_pGameContext->m_pCLRenderer->Init(pGameContext);
}

bool Application::HandleInput()
{
	SDL_Event e{};
	while (SDL_PollEvent(&e)) {

		//early exit
		if (e.type == SDL_QUIT)
		{
			return false;
			break;
		}
	}
	return true;
}

void Application::Render()
{
	m_pGameContext->m_pGLRenderer->Begin();
		m_pGameContext->m_pGamemode->Draw(m_pGameContext);
	m_pGameContext->m_pGLRenderer->End();

	m_pGameContext->m_pCLRenderer->Draw(m_pGameContext);
}

void Application::OnUpdate(GameContext* pGameContext)
{
	m_pGameContext->m_pGamemode->Update(pGameContext);
}