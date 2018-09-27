#pragma once
#include "Scenegraph\GameObject.h"

class BaseController;
class BasePawn : public GameObject
{
public:
	BasePawn();
	virtual ~BasePawn();

	void SetPossessor(BaseController* pController);
	BaseController* GetController() const;
private:
	BaseController* m_pController;
};