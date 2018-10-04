#pragma once
struct Vertex
{
	Vertex(glm::vec3 pos)
		:Position(pos) {};

	Vertex(float x, float y, float z)
		:Vertex(glm::vec3(x, y, z)) {};

	glm::vec3 Position;
};

struct VertexPosCol : public Vertex
{
	glm::vec3 Color;
};

struct VertexPosTex : public Vertex
{
	glm::vec2 TexCoords;
};

struct VertexPosColNorm : public VertexPosCol
{
	glm::vec3 Normal;
};

struct VertexPosTexNorm : public VertexPosTex
{
	glm::vec3 Normal;
};