#include "FrameworkPCH.h"
#include "BasePlayerState.h"

BasePlayerState::BasePlayerState()
	:BaseControllerState(), m_Score()
{
}

BasePlayerState::~BasePlayerState()
{
}

int BasePlayerState::GetScore() const
{
	return m_Score;
}

void BasePlayerState::SetScore(int score)
{
	m_Score = score;
}

void BasePlayerState::IncrementScore(int incrementAmount)
{
	m_Score += incrementAmount;
}

void BasePlayerState::DecrementScore(int decrementAmount)
{
	m_Score -= decrementAmount;
}