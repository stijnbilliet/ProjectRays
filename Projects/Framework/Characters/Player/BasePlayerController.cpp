#include "FrameworkPCH.h"
#include "BasePlayerController.h"
#include "Input/InputManager.h"
#include "BasePlayerState.h"
#include "Game/BaseGamemode.h"

using Super = BaseController;

BasePlayerController::BasePlayerController()
	:BaseController()
{
	SetState(new BasePlayerState());
}

BasePlayerController::~BasePlayerController()
{
}

void BasePlayerController::OnInit()
{
	Super::OnInit();

	auto numPlayers = GetGamemode()->GetPlayers().size();
	m_Index = uint8_t(numPlayers - 1);
}

void BasePlayerController::HandleInput(SDL_Event* e)
{
	GetGamemode()->GetInputManager()->HandleInput(e, m_Index);
}