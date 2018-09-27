#pragma once
#include "Characters\BaseControllerState.h"

class BasePlayerState : public BaseControllerState
{
public:
	BasePlayerState();
	virtual ~BasePlayerState();

	int GetScore() const;
	void SetScore(int score);
	void IncrementScore(int incrementAmount);
	void DecrementScore(int decrementAmount);

private:
	int m_Score;
};