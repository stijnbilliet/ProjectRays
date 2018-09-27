#pragma once
#include "GUI/Text/TextLabel.h"

class TextureData;
class MeshComponent;
class FPSCounter : public TextLabel
{
public:
	FPSCounter(int ptSize, const glm::vec4& txtColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	virtual ~FPSCounter();

private:
	void PreInit() override;
	void OnInit() override;
	void OnUpdate(float deltaTime) override;

	float m_ElapsedSec;
};