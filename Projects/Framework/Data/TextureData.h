#pragma once
#include "Patterns/BaseObject.h"

class Renderer;
class TextureData : public BaseObject
{
public:
	TextureData(const std::string& path);
	TextureData(const std::string& text, const std::string& fontPath, int ptSize, const glm::vec4& txtColor);

	virtual ~TextureData();

	const GLuint& GetTextureId();
	const glm::vec2& GetTextureSize();

private:
	void CreateFromSurface(SDL_Surface *pSurface);

	GLuint m_Id;
	glm::vec2 m_Size;
};