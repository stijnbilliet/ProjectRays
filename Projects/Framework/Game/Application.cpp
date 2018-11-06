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
	PropertyManager::GetInstance().Add("renderer_viewwidth", "1600");
	PropertyManager::GetInstance().Add("renderer_viewheight", "900");
	PropertyManager::GetInstance().Add("windowtitle", "ProjectRays(OpenGL+RadeonRays)");
}

void Application::OnInit(GameContext* pGameContext)
{
	m_pGameContext->m_pGLRenderer->OnInit(pGameContext);
	m_pGameContext->m_pCLRenderer->Init(pGameContext);
	m_pGameContext->m_pCLRenderer->InitCL(pGameContext);
	m_pGameContext->m_pGLRenderer->PostInit(pGameContext);
	m_pGameContext->m_pCLRenderer->InitRadeonRays(pGameContext);
	m_pGameContext->m_pCLRenderer->InitKernels(pGameContext);
	m_pGameContext->m_pGamemode->Init(pGameContext);
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

		m_pGameContext->m_pInputManager->HandleInput(e);
	}
	return true;
}

void Application::Render()
{
	auto pGLRenderer = m_pGameContext->m_pGLRenderer;
	auto pCLRenderer = m_pGameContext->m_pCLRenderer;
	auto pGameMode = m_pGameContext->m_pGamemode;

	pGLRenderer->Begin();
	pGameMode->Draw(m_pGameContext);
	pCLRenderer->RaytracedShadows(m_pGameContext);
	pGLRenderer->LightPass();
	pGLRenderer->End();
}

void Application::OnUpdate(GameContext* pGameContext)
{
	m_pGameContext->m_pGamemode->Update(pGameContext);
}