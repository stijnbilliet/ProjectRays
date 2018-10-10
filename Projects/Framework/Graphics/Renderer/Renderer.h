#pragma once
#include "Patterns/SingleInstance.h"

class ShaderProgram;
class Light;
class Renderer : public SingleInstance<Renderer>
{
public:
	Renderer();
	virtual ~Renderer();

	void Begin();
	void End();

	unsigned int GetGBuffer() const;
	ShaderProgram* GetLightDrawer() const;

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

	glm::vec3 m_DirectionalPos;
	glm::vec3 m_DirectionalCol;

	SDL_GLContext m_Context;
	SDL_Window* m_pWindow;
	bool m_Vsync;
};