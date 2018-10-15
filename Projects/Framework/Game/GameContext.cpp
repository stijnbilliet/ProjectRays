#include "FrameworkPCH.h"
#include "GameContext.h"

GameContext::GameContext()
{
}

GameContext::~GameContext()
{
	safe_delete(m_pLightManager);
	safe_delete(m_pSceneManager);
	safe_delete(m_pInputManager);
	safe_delete(m_pGLRenderer);
	safe_delete(m_pCLRenderer);
}
