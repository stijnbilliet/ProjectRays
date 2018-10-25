#pragma once
#include "Patterns/SingleInstance.h"

class ShaderProgram;
class GL_Renderer : public SingleInstance<GL_Renderer>
{
public:
	GL_Renderer();
	virtual ~GL_Renderer();

	void Begin();
	void LightPass();
	void End();

	const SDL_GLContext& GetGLContext() const;
	SDL_Window* GetWindow() const;

	unsigned int GetGBuffer() const;
	unsigned int GetWorldPosBuffer() const;
	unsigned int GetNormalBuffer() const;
	unsigned int GetLightBuffer() const;
	const glm::vec3& GetDirectionalLightPos() const;

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
	ShaderProgram* m_pSimpleShader;

	glm::vec3 m_DirectionalPos;
	glm::vec3 m_DirectionalCol;

	SDL_GLContext m_Context;
	SDL_Window* m_pWindow;
	bool m_Vsync;
};