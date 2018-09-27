#include "FrameworkPCH.h"
#include "BaseGamemode.h"
#include "Scenegraph/SceneManager.h"
#include "Characters/Player/BasePlayerController.h"
#include "Characters/BasePawn.h"
#include "Input/InputManager.h"
#include "GUI/UserInterface.h"

BaseGamemode::BaseGamemode()
	:BaseObject()
{
	m_pInputManager = new InputManager();
	m_pUserInterface = new UserInterface();
	m_pSceneManager = new SceneManager();

	AddPlayer(new BasePlayerController());
}

BaseGamemode::~BaseGamemode()
{
	for (auto pPly : m_pPlayers)
	{
		safe_delete(pPly);
	}

	safe_delete(m_pUserInterface);
	safe_delete(m_pSceneManager);
	safe_delete(m_pInputManager);
}

void BaseGamemode::OnDraw(Renderer* pContext) const
{
	m_pSceneManager->Draw(pContext);
	m_pUserInterface->Draw(pContext);

	for (auto pPly : m_pPlayers)
	{
		pPly->Draw(pContext);
	}

	for (auto pBot : m_pPlayers)
	{
		pBot->Draw(pContext);
	}
}

std::vector<BasePlayerController*> BaseGamemode::GetPlayers() const
{
	return m_pPlayers;
}

InputManager* BaseGamemode::GetInputManager() const
{
	return m_pInputManager;
}

SceneManager* BaseGamemode::GetSceneManager() const
{
	return m_pSceneManager;
}

UserInterface* BaseGamemode::GetUserInterface() const
{
	return m_pUserInterface;
}

BasePlayerController* BaseGamemode::GetPlayerController(uint8_t plyIndex) const
{
	for (auto pPly : m_pPlayers)
	{
		if (pPly->GetIndex() == plyIndex)
		{
			return pPly;
		}
	}
	return nullptr;
}

void BaseGamemode::AddPlayer(BasePlayerController* pNewPly)
{
	if (pNewPly)
	{
		pNewPly->SetGamemode(this);
		m_pPlayers.push_back(pNewPly);
	}
}

void BaseGamemode::OnUpdate(float deltaTime)
{
	m_pSceneManager->Update(deltaTime);
	m_pUserInterface->Update(deltaTime);

	for (auto pPly : m_pPlayers)
	{
		pPly->Update(deltaTime);
	}
}

void BaseGamemode::OnInit()
{
	m_pInputManager->Init();
	m_pSceneManager->Init();
	m_pUserInterface->Init();

	for (auto pPly : m_pPlayers)
	{
		pPly->Init();
	}

	for (auto pBot : m_pPlayers)
	{
		pBot->Init();
	}
}

void BaseGamemode::OnFixedUpdate(float stepRate)
{
	m_pSceneManager->FixedUpdate(stepRate);
}

void BaseGamemode::AutoPosses(BasePawn* pPawn)
{
	for (auto pPly : m_pPlayers)
	{
		if (pPly->Possess(pPawn)) break;
	}
}