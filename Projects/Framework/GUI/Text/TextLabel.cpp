#include "FrameworkPCH.h"
#include "TextLabel.h"
#include "Scenegraph/Scene.h"

using Super = UIElement;

TextLabel::TextLabel(int ptSize, const glm::vec4& txtColor)
	:UIElement(), m_ptSize(ptSize), m_Color(txtColor), m_Text("N/A")
{
}

TextLabel::~TextLabel()
{
}

void TextLabel::SetText(std::string text)
{
	m_PreviousText = m_Text;
	m_Text = text;
}

std::string TextLabel::GetText() const
{
	return m_Text;
}

void TextLabel::PreInit()
{
	Super::PreInit();

}

void TextLabel::OnInit()
{
	Super::OnInit();

}

void TextLabel::OnDraw(Renderer * pContext) const
{
	Super::OnDraw(pContext);

	if (m_Text != m_PreviousText)
	{
		//Create new texturedata
		auto pTextureData = std::make_shared<TextureData>(m_Text, "../../Assets/Consolas.ttf", m_ptSize, m_Color);

		//Create new meshdata
	}
}