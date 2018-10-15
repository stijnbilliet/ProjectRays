#pragma once
#include "Patterns/SingleInstance.h"

class ShaderProgram;
class GL_Renderer : public SingleInstance<GL_Renderer>
{
public:
	GL_Renderer();
	virtual ~GL_Renderer();

	void Begin();
	void End();

	unsigned int GetGBuffer() const;
	ShaderProgram* GetLightDrawer() const;

	virtual void OnInit(GameContext* pGameContext) override;
	virtual void PostInit(GameContext* pGameContext) override;
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

	glm::vec3 m_DirectionalPos;
	glm::vec3 m_DirectionalCol;

	SDL_GLContext m_Context;
	SDL_Window* m_pWindow;
	bool m_Vsync;
};