#pragma once
class GL_Renderer;
class SceneManager;
class BaseGamemode;
class CL_Renderer : public SingleInstance<CL_Renderer>
{
public:
	CL_Renderer(BaseGamemode* pGamemode);
	virtual ~CL_Renderer();

	virtual void OnInit() override;
	virtual void PreDraw(GL_Renderer* pContext) const override;
	virtual void OnDraw(GL_Renderer* pContext) const override;
private:
	const SceneManager* m_pSceneManager;
};