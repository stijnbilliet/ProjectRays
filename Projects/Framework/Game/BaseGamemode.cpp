#include "FrameworkPCH.h"
#include "BaseGamemode.h"

BaseGamemode::BaseGamemode()
	:BaseObject()
{
}

BaseGamemode::~BaseGamemode()
{
}

void BaseGamemode::OnDraw(GameContext* pContext) const
{
	pContext->m_pSceneManager->Draw(pContext);
}

void BaseGamemode::OnUpdate(GameContext* pContext)
{
	pContext->m_pSceneManager->Update(pContext);
}