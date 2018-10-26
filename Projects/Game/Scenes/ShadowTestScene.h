#pragma once
class ShadowTestScene : public Scene
{
public:
	ShadowTestScene();
	virtual ~ShadowTestScene();

	virtual void PreInit(GameContext* pContext) override;
	virtual void PostInit(GameContext* pContext) override;
private:
	Model* m_pModel;
};