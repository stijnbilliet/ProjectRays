#include "FrameworkPCH.h"
#include "BaseObject.h"

BaseObject::BaseObject()
{
}

BaseObject::~BaseObject()
{
}

void BaseObject::Init()
{
	PreInit();
	{
		OnInit();
	}
	PostInit();
}

void BaseObject::FixedUpdate(float stepRate)
{
	PreFixedUpdate(stepRate);
	{

		OnFixedUpdate(stepRate);
	}
	PostFixedUpdate(stepRate);
}

void BaseObject::Update(float deltaTime)
{
	PreUpdate(deltaTime);
	{

		OnUpdate(deltaTime);
	}
	PostUpdate(deltaTime);
}

void BaseObject::Draw(Renderer* pContext) const
{
	PreDraw(pContext);
	{
		OnDraw(pContext);
	}
	PostDraw(pContext);
}

void BaseObject::PreInit()
{
}

void BaseObject::OnInit()
{
}

void BaseObject::PostInit()
{
}

void BaseObject::PreFixedUpdate(float stepRate)
{
	UNREFERENCED_PARAMETER(stepRate);
}

void BaseObject::OnFixedUpdate(float stepRate)
{
	UNREFERENCED_PARAMETER(stepRate);
}

void BaseObject::PostFixedUpdate(float stepRate)
{
	UNREFERENCED_PARAMETER(stepRate);
}

void BaseObject::PreUpdate(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);
}

void BaseObject::OnUpdate(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);
}

void BaseObject::PostUpdate(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);
}

void BaseObject::PreDraw(Renderer* pContext) const
{
	UNREFERENCED_PARAMETER(pContext);
}

void BaseObject::OnDraw(Renderer * pContext) const
{
	UNREFERENCED_PARAMETER(pContext);
}

void BaseObject::PostDraw(Renderer* pContext) const
{
	UNREFERENCED_PARAMETER(pContext);
}