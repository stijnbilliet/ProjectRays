#include "FrameworkPCH.h"
#include "Game.h"
#include <chrono>
#include "Graphics\Renderer\Renderer.h"
#include "Scenegraph/SceneManager.h"
#include "Game/BaseGamemode.h"
#include "Characters/Player/BasePlayerController.h"
#include "PropertyManager.h"

Game::Game(BaseGamemode* pGamemode, Renderer* pRenderer, const std::vector<std::string>& args)
	:m_pGameMode(pGamemode), m_pRenderer(pRenderer), m_Args(args)
{
	ParseCmdLineArgs();
	FillPropertyManager();
}

Game::~Game()
{
	safe_delete(m_pGameMode);
	safe_delete(m_pRenderer);
}

void Game::Run()
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

void Game::ParseCmdLineArgs()
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

void Game::FillPropertyManager()
{
	PropertyManager::GetInstance().Add("renderer_viewwidth", "854");
	PropertyManager::GetInstance().Add("renderer_viewheight", "480");
	PropertyManager::GetInstance().Add("windowtitle", "ProjectRays(OpenGL+RadeonRays)");
}

void Game::OnInit()
{
	m_pRenderer->Init();
	m_pGameMode->Init();
}

bool Game::HandleInput()
{
	SDL_Event e{};
	while (SDL_PollEvent(&e)) {

		//early exit
		if (e.type == SDL_QUIT)
		{
			return false;
			break;
		}

		//check all players
		for (auto ply : m_pGameMode->m_pPlayers)
		{
			ply->HandleInput(&e);
		}
	}
	return true;
}

void Game::Render()
{
	m_pRenderer->Begin();
		m_pGameMode->Draw(m_pRenderer);
	m_pRenderer->End();
}

void Game::OnUpdate(float deltaTime)
{
	m_pGameMode->Update(deltaTime);
}