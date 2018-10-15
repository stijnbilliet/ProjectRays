#pragma once

class CameraComponent;
class LightManager;
class GL_Renderer;
class CL_Renderer;
class SceneManager;
class InputManager;
class BaseGamemode;
class GameContext
{
public:
	GameContext();
	~GameContext();

	BaseGamemode* m_pGamemode;
	CameraComponent* m_pActiveCamera;
	LightManager* m_pLightManager;
	SceneManager* m_pSceneManager;
	InputManager* m_pInputManager;
	GL_Renderer* m_pGLRenderer;
	CL_Renderer* m_pCLRenderer;
	float elapsedSec;
};