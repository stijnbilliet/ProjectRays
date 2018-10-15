#include "GamePCH.h"
#include "GMSandbox.h"
#include "Scenes/RenderTestScene.h"
#include "Gamemode/InputActions.hpp"
#include "Components/CameraComponent.h"

using Super = BaseGamemode;

GMSandbox::GMSandbox()
	:BaseGamemode()
{
}

GMSandbox::~GMSandbox()
{

}

void GMSandbox::OnUpdate(GameContext* pContext)
{
	Super::OnUpdate(pContext);
}

void GMSandbox::PreInit()
{
	Super::OnInit();

	//Level
	auto pLevel = new RenderTestScene();
	m_pSceneManager->Add(pLevel);
	m_pSceneManager->SetActive("RenderTestScene");
}