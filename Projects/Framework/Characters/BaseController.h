#pragma once
#include "Patterns/BaseObject.h"

class BasePawn;
class BaseControllerState;
class BaseGamemode;
class BaseController : public BaseObject
{
public:
	BaseController();
	virtual ~BaseController();

	bool Possess(BasePawn* pPawn);
	BasePawn* GetPawn() const;

	void SetState(BaseControllerState* pState);
	BaseControllerState* GetState() const;

	void SetGamemode(BaseGamemode* pGameMode);
	BaseGamemode* GetGamemode() const;

	int32_t GetIndex() const;
protected:
	int32_t m_Index;
private:
	BasePawn* m_pPawn;
	BaseGamemode* m_pGamemode;
	BaseControllerState* m_pState;
};