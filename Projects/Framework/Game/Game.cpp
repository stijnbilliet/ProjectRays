#include "FrameworkPCH.h"
#include "Game.h"
#include <chrono>
#include "Graphics/Renderer.h"
#include "Scenegraph/SceneManager.h"
#include "Game/BaseGamemode.h"
#include "Characters/Player/BasePlayerController.h"

Game::Game(BaseGamemode* pGamemode, Renderer* pRenderer)
	:m_Accumulator(0.0f), m_StepRate(1/60.0f), m_pGameMode(pGamemode), m_pRenderer(pRenderer)
{
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

		//Physics loop
		//Disabled since we don't need physics (is 2d physics anyway)
		//std::async(std::launch::async, &Game::Integrate, this, deltaTime);

		//Game Logic
		std::async(std::launch::async, &Game::UpdateGame, this, deltaTime);

		//Render
		Render();
	}

	Clean();
}

void Game::OnInit()
{
	m_pRenderer->Init();
	m_pGameMode->Init();
}

void Game::OnFixedUpdate(float stepRate)
{
	m_pGameMode->FixedUpdate(stepRate);
}

void Game::Clean()
{
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
	m_pRenderer->Clear();
		m_pGameMode->Draw(m_pRenderer);
	m_pRenderer->Present();
}

void Game::UpdateGame(float deltaTime)
{
	//Locks upon construction, unlocks when leaving scope
	std::lock_guard<std::mutex> lock(m_PhysicsMutex);
	Update(deltaTime);
}

void Game::Integrate(float deltaTime)
{
	m_Accumulator += deltaTime;
	while (m_Accumulator >= m_StepRate)
	{
		//Locks upon construction, unlocks when leaving scope
		std::lock_guard<std::mutex> lock(m_PhysicsMutex);
		FixedUpdate(m_StepRate);
		m_Accumulator -= m_StepRate;
	}
}

void Game::OnUpdate(float deltaTime)
{
	m_pGameMode->Update(deltaTime);
}