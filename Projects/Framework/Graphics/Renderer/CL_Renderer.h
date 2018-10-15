#pragma once
class GL_Renderer;
class SceneManager;
class BaseGamemode;
class GameContext;
class CL_Renderer : public SingleInstance<CL_Renderer>
{
public:
	CL_Renderer();
	virtual ~CL_Renderer();

	virtual void OnInit(GameContext* pGameContext) override;
	virtual void PreDraw(GameContext* pContext) const override;
	virtual void OnDraw(GameContext* pContext) const override;
private:
	RadeonRays::IntersectionApi* m_pApi;
};