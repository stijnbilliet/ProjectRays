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

	unsigned int GetGBuffer() const;

	virtual void OnInit() override;
	virtual void PostInit() override;
private:
	void RenderQuad();
	void ImGuiOnDraw();
	unsigned int quadVAO = 0;
	unsigned int quadVBO;

	unsigned int m_gBuffer;
	unsigned int gPosition, gAlbedo, gNormal, gLightAcc;

	int m_ScrWidth;
	int m_ScrHeight;

	std::string m_WindowTitle;
	ShaderProgram* m_pLightPass;

	SDL_GLContext m_Context;
	SDL_Window* m_pWindow;
	bool m_Vsync;
};