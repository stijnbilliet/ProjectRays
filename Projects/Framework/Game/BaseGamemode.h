#pragma once
#include "Patterns/BaseObject.h"

class SceneManager; class GL_Renderer; class InputManager;
class BaseGamemode : public BaseObject
{
public:
	friend class Application;

	BaseGamemode();
	virtual ~BaseGamemode();

	void OnInit() override;
	void OnUpdate(float deltaTime) override;
	void OnDraw(GL_Renderer* pContext) const override;

	//System accessors
	InputManager* GetInputManager() const;
	SceneManager* GetSceneManager() const;
protected:
	InputManager* m_pInputManager;
	SceneManager* m_pSceneManager;
};