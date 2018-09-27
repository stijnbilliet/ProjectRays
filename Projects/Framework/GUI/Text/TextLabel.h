#pragma once
#include "GUI\UIElement.h"

class MeshComponent;
class TextLabel : public UIElement
{
public:
	TextLabel(int ptSize, const glm::vec4& txtColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	virtual ~TextLabel();

	void SetText(std::string text);
	std::string GetText() const;
protected:
	virtual void PreInit() override;
	virtual void OnInit() override;
	virtual void OnDraw(Renderer* pContext) const override;

	std::string m_Text;
	std::string m_PreviousText;
	glm::vec4 m_Color;
	int m_ptSize;
};