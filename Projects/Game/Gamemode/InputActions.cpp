#include "GamePCH.h"
#include "InputActions.h"
#include "GMSandbox.h"

Forward::Forward(GameContext* pContext)
	:m_pGameContext(pContext)
{
}

void Forward::execute()
{
	if(!m_pTransformComp) m_pTransformComp = m_pGameContext->m_pActiveCamera->GetGameObject()->GetTransform();

	auto prevTranslation = m_pTransformComp->GetWorldPosition();
	auto newTranslation = prevTranslation + (m_pTransformComp->GetForward() *20.0f* m_pGameContext->elapsedSec);
	m_pTransformComp->Translate(newTranslation);
}

Backward::Backward(GameContext * pContext)
	:m_pGameContext(pContext)
{
}

void Backward::execute()
{
	if(!m_pTransformComp) m_pTransformComp = m_pGameContext->m_pActiveCamera->GetGameObject()->GetTransform();

	auto prevTranslation = m_pTransformComp->GetWorldPosition();
	auto newTranslation = prevTranslation - (m_pTransformComp->GetForward() *20.0f* m_pGameContext->elapsedSec);
	m_pTransformComp->Translate(newTranslation);
}

Left::Left(GameContext * pContext)
	:m_pGameContext(pContext)
{
}

void Left::execute()
{
	if (!m_pTransformComp) m_pTransformComp = m_pGameContext->m_pActiveCamera->GetGameObject()->GetTransform();

	auto prevTranslation = m_pTransformComp->GetWorldPosition();
	auto newTranslation = prevTranslation - (m_pTransformComp->GetRight() *20.0f* m_pGameContext->elapsedSec);
	m_pTransformComp->Translate(newTranslation);
}

Right::Right(GameContext * pContext)
	:m_pGameContext(pContext)
{
}

void Right::execute()
{
	if (!m_pTransformComp) m_pTransformComp = m_pGameContext->m_pActiveCamera->GetGameObject()->GetTransform();

	auto prevTranslation = m_pTransformComp->GetWorldPosition();
	auto newTranslation = prevTranslation + (m_pTransformComp->GetRight() *20.0f* m_pGameContext->elapsedSec);
	m_pTransformComp->Translate(newTranslation);
}

Up::Up(GameContext * pContext)
	:m_pGameContext(pContext)
{
}

void Up::execute()
{
	if (!m_pTransformComp) m_pTransformComp = m_pGameContext->m_pActiveCamera->GetGameObject()->GetTransform();

	auto prevTranslation = m_pTransformComp->GetWorldPosition();
	auto newTranslation = prevTranslation + (m_pTransformComp->GetUp() *20.0f* m_pGameContext->elapsedSec);
	m_pTransformComp->Translate(newTranslation);
}

Down::Down(GameContext * pContext)
	:m_pGameContext(pContext)
{
}

void Down::execute()
{
	if (!m_pTransformComp) m_pTransformComp = m_pGameContext->m_pActiveCamera->GetGameObject()->GetTransform();

	auto prevTranslation = m_pTransformComp->GetWorldPosition();
	auto newTranslation = prevTranslation - (m_pTransformComp->GetUp() *20.0f* m_pGameContext->elapsedSec);
	m_pTransformComp->Translate(newTranslation);
}

LookAround::LookAround(GameContext * pContext)
	:m_pGameContext(pContext)
{
}

void LookAround::execute()
{
	if (!m_pTransformComp) m_pTransformComp = m_pGameContext->m_pActiveCamera->GetGameObject()->GetTransform();
	glm::vec2 mousePos = m_pGameContext->m_pInputManager->GetMousePos();

	auto deltaPos = glm::vec2();
	deltaPos.x = mousePos.x - m_PrevMousePos.x;
	deltaPos.y = m_PrevMousePos.y - mousePos.y;
	m_PrevMousePos = mousePos;

	auto prevRotation = m_pTransformComp->GetWorldRotation();
	auto angles = glm::eulerAngles(prevRotation);
	auto Yaw = angles.y + (deltaPos.x * 0.005f);
	auto Pitch = angles.x + (deltaPos.y * 0.005f);
	
	glm::quat qPitch = glm::angleAxis(Pitch, glm::vec3(1, 0, 0));
	glm::quat qYaw = glm::angleAxis(Yaw, glm::vec3(0, 1, 0));

	glm::quat rotQuat = qYaw * qPitch;
	m_pTransformComp->Rotate(rotQuat);
}

ToggleLightCycle::ToggleLightCycle()
{
}

void ToggleLightCycle::execute()
{
	Logger::GetInstance().LogInfo("Light cycle started \n");
	GMSandbox::_RunLightCycle = !GMSandbox::_RunLightCycle;
}

AutoPanCamera::AutoPanCamera()
{
}

void AutoPanCamera::execute()
{
	Logger::GetInstance().LogInfo("Camera auto panning on \n");
	GMSandbox::_AutoPanCamera = !GMSandbox::_AutoPanCamera;
}