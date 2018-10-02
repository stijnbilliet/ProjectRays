#pragma once
struct Vertex
{
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