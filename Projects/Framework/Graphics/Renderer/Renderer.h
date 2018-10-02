#pragma once
#include "Patterns/SingleInstance.h"

class ShaderProgram;
class Renderer : public SingleInstance<Renderer>
{
public:
	Renderer();
	virtual ~Renderer();

	//TODO: Renderer should draw instead of game

	void Begin();
	void End();

	virtual void OnInit() override;
	virtual void PostInit() override;
private:
	//TEMP
	ShaderProgram* m_pShaderProgram;
	unsigned int VBO, VAO, EBO;

	SDL_GLContext m_Context;
	SDL_Window* m_pWindow;
	bool m_Vsync;
};