#include "GamePCH.h"
#include "GMSandbox.h"
#include "Scenes/RenderTestScene.h"

using Super = BaseGamemode;

GMSandbox::GMSandbox()
	:BaseGamemode()
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
	Super::OnInit();

	//Level
	auto pLevel = new RenderTestScene();
	m_pSceneManager->Add(pLevel);
	m_pSceneManager->SetActive("RenderTestScene");
}