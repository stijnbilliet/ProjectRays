#include "GamePCH.h"
#include "GMSandbox.h"
#include "Scenes/RenderTestScene.h"
#include "Gamemode/InputActions.h"
#include "Components/CameraComponent.h"
#include "Scenes/SponzaScene.h"
#include "Scenes/ShadowTestScene.h"

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
	auto pRenderTest = new RenderTestScene();
	auto pSponzaScene = new SponzaScene();
	auto pShadowTestScene = new ShadowTestScene();

	pContext->m_pSceneManager->Add(pRenderTest);
	pContext->m_pSceneManager->Add(pSponzaScene);
	pContext->m_pSceneManager->Add(pShadowTestScene);

	pContext->m_pSceneManager->SetActive("ShadowTestScene");

	//Input
	auto pForward = new Forward(pContext);
	pContext->m_pInputManager->AddInputAction(EventFactory::CreateDesc(SDL_KEYDOWN, SDLK_w), pForward);

	auto pBackward = new Backward(pContext);
	pContext->m_pInputManager->AddInputAction(EventFactory::CreateDesc(SDL_KEYDOWN, SDLK_s), pBackward);

	auto pLeft = new Left(pContext);
	pContext->m_pInputManager->AddInputAction(EventFactory::CreateDesc(SDL_KEYDOWN, SDLK_a), pLeft);

	auto pRight = new Right(pContext);
	pContext->m_pInputManager->AddInputAction(EventFactory::CreateDesc(SDL_KEYDOWN, SDLK_d), pRight);

	auto pUp = new Up(pContext);
	pContext->m_pInputManager->AddInputAction(EventFactory::CreateDesc(SDL_KEYDOWN, SDLK_q), pUp);

	auto pDown = new Down(pContext);
	pContext->m_pInputManager->AddInputAction(EventFactory::CreateDesc(SDL_KEYDOWN, SDLK_e), pDown);

	auto pLook = new LookAround(pContext);
	auto output = EventFactory::CreateDesc(SDL_KEYDOWN, SDLK_p);
	pContext->m_pInputManager->AddInputAction(output, pLook);
}