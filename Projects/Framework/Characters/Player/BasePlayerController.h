#pragma once
#include "../BaseController.h"

class BaseGamemode;
class BasePlayerController : public BaseController
{
public:
	BasePlayerController();
	virtual ~BasePlayerController();

	virtual void OnInit() override;
	void HandleInput(SDL_Event* e);
private:
};