#pragma once
enum TextureType
{
	DIFFUSE,
	SPECULAR
};

struct Texture
{
	Texture() = default;
	Texture(const char *path);

	unsigned int id;
	TextureType type;
	std::string path;
};