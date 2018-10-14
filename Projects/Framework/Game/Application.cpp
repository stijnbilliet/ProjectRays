#include "FrameworkPCH.h"
#include "Application.h"
#include "Graphics/Renderer/CL_Renderer.h"

Application::Application(BaseGamemode* pGamemode, GL_Renderer* pRenderer, const std::vector<std::string>& args)
	:m_pGameMode(pGamemode), m_pRenderer(pRenderer), m_Args(args), m_pRaytracer(new CL_Renderer(pGamemode))
{
	ParseCmdLineArgs();
	FillPropertyManager();
}

Application::~Application()
{
	safe_delete(m_pGameMode);
	safe_delete(m_pRenderer);
	safe_delete(m_pRaytracer);
}

void Application::Run()
{
	Init();

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
		Update(deltaTime);

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

void Application::OnInit()
{
	m_pRenderer->Init();
	m_pGameMode->Init();
	m_pRaytracer->Init();
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
	m_pRenderer->Begin();
		m_pGameMode->Draw(m_pRenderer);
	m_pRenderer->End();

	m_pRaytracer->Draw(m_pRenderer);
}

void Application::OnUpdate(float deltaTime)
{
	m_pGameMode->Update(deltaTime);
}