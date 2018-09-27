#include "GamePCH.h"
#include "GMSandbox.h"

using Super = BaseGamemode;

GMSandbox::GMSandbox()
	:BaseGamemode(), m_pCornelScene(nullptr)
{
}

GMSandbox::~GMSandbox()
{

}

void GMSandbox::OnUpdate(float deltaTime)
{
	Super::OnUpdate(deltaTime);

}

void GMSandbox::PreInit()
{
	Super::PreInit();

}

void GMSandbox::PostInit()
{
	Super::PostInit();

}

void GMSandbox::InitInput(unsigned int playerIndex)
{

}