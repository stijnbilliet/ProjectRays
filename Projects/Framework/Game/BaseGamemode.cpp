#include "FrameworkPCH.h"
#include "BaseGamemode.h"

BaseGamemode::BaseGamemode()
	:BaseObject()
{
	m_pInputManager = new InputManager();
	m_pSceneManager = new SceneManager();
}

BaseGamemode::~BaseGamemode()
{
	safe_delete(m_pSceneManager);
	safe_delete(m_pInputManager);
}

void BaseGamemode::OnDraw(GameContext* pContext) const
{
	m_pSceneManager->Draw(pContext);
}

InputManager* BaseGamemode::GetInputManager() const
{
	return m_pInputManager;
}

SceneManager* BaseGamemode::GetSceneManager() const
{
	return m_pSceneManager;
}

void BaseGamemode::OnUpdate(GameContext* pContext)
{
	m_pSceneManager->Update(pContext);
}

void BaseGamemode::OnInit()
{


	m_pInputManager->Init();
	m_pSceneManager->Init();
}