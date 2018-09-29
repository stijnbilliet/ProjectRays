#include "FrameworkPCH.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Graphics\Renderer\Renderer.h"

SceneManager::SceneManager()
	:BasePanelManager<Scene>()
{

};

SceneManager::~SceneManager()
{
}

void SceneManager::OnFixedUpdate(float stepRate)
{
	m_pCurPanel->FixedUpdate(stepRate);
}