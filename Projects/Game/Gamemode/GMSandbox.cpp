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

void GMSandbox::PreInit(GameContext* pContext)
{
	Super::OnInit(pContext);

	//Level
	auto pLevel = new RenderTestScene();
	pContext->m_pSceneManager->Add(pLevel);
	pContext->m_pSceneManager->SetActive("RenderTestScene");
}