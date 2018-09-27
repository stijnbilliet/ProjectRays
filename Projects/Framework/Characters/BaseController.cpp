#include "FrameworkPCH.h"
#include "BaseController.h"
#include "BaseControllerState.h"
#include "BasePawn.h"

BaseController::BaseController()
	:m_pPawn(), m_pState(), m_Index(-1), m_pGamemode(nullptr)
{
}

BaseController::~BaseController()
{
	safe_delete(m_pState);
}

bool BaseController::Possess(BasePawn* pPawn)
{
	if (m_pPawn) return false;
	m_pPawn = pPawn;
	m_pPawn->SetPossessor(this);
	return true;
}

BasePawn* BaseController::GetPawn() const
{
	return m_pPawn;
}

void BaseController::SetState(BaseControllerState * pState)
{
	safe_delete(m_pState);
	m_pState = pState;
}

BaseControllerState* BaseController::GetState() const
{
	return m_pState;
}

void BaseController::SetGamemode(BaseGamemode * pGameMode)
{
	m_pGamemode = pGameMode;
}

BaseGamemode * BaseController::GetGamemode() const
{
	return m_pGamemode;
}

int32_t BaseController::GetIndex() const
{
	return m_Index;
}
