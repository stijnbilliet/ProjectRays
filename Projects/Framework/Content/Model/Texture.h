#pragma once
enum TextureType
{
	DIFFUSE,
	SPECULAR,
	NORMAL,
	OPACITY,
	AMBIENT
};

class Texture
{
public:
	Texture() = default;
	Texture(const char *path, TextureType type);

	unsigned int id;
	TextureType type;
	std::string path;
};