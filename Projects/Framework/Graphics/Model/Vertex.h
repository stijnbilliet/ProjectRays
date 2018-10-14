#pragma once
struct Vertex
{
	Vertex() = default;

	Vertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 texCoord)
		:Position(pos), Normal(norm), TexCoords(texCoord){};

	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};