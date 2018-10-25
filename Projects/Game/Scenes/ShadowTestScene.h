#pragma once
class ShadowTestScene : public Scene
{
public:
	ShadowTestScene();
	virtual ~ShadowTestScene();

	virtual void PreInit(GameContext* pContext) override;
private:
	Model* m_pModel;
};