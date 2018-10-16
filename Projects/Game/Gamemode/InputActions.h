#pragma once
#include "Patterns/Command.h"

class GameContext;
class Forward : public Command
{
public:
	Forward(GameContext* pContext);

	virtual void execute() override;
private:
	GameContext* m_pGameContext;
	TransformComponent* m_pTransformComp;
};

class Backward : public Command
{
public:
	Backward(GameContext* pContext);

	virtual void execute() override;
private:
	GameContext* m_pGameContext;
	TransformComponent* m_pTransformComp;
};

class Left : public Command
{
public:
	Left(GameContext* pContext);

	virtual void execute() override;
private:
	GameContext* m_pGameContext;
	TransformComponent* m_pTransformComp;
};

class Right : public Command
{
public:
	Right(GameContext* pContext);

	virtual void execute() override;
private:
	GameContext * m_pGameContext;
	TransformComponent* m_pTransformComp;
};

class Up : public Command
{
public:
	Up(GameContext* pContext);

	virtual void execute() override;
private:
	GameContext * m_pGameContext;
	TransformComponent* m_pTransformComp;
};

class Down : public Command
{
public:
	Down(GameContext* pContext);

	virtual void execute() override;
private:
	GameContext * m_pGameContext;
	TransformComponent* m_pTransformComp;
};

class LookAround : public Command
{
public:
	LookAround(GameContext* pContext);

	virtual void execute() override;
private:
	GameContext * m_pGameContext;
	TransformComponent* m_pTransformComp;
	glm::vec2 m_PrevMousePos;
};