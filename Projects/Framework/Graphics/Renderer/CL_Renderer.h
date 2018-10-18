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

	RadeonRays::IntersectionApi* GetRaysAPI() const;
private:
	void InitCL(GameContext* pGameContext);
	void InitRadeonRays(GameContext* pGameContext);
	void InitKernels(GameContext* pGameContext);

	RadeonRays::IntersectionApi* m_pApi;
	CLWContext m_CLContext; //runtime interface between cpu and gpu
	CLWProgram m_ShadowProgram; //one or more kernels bundled in one
	CLWDevice m_CLDevice;

	std::vector<RadeonRays::ray> m_Rays;
	int m_ScreenWidth;
	int m_ScreenHeight;
	std::string m_AssetPath;
};