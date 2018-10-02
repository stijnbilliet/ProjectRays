#pragma once
#include "Patterns/BaseObject.h"
#include "Content/Model/Vertex.h"

class Mesh
{
public:
	Mesh(std::vector<Vertex> verts, std::vector<unsigned int> indices);
	virtual ~Mesh();

	unsigned int GetVertexArrayObj() const;
	unsigned int GetVertexBufferObj() const;
	unsigned int GetElementBufferObj() const;

	const std::vector<Vertex>& GetVertices() const;
	const std::vector<unsigned int>& GetIndices() const;
private:
	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;

	unsigned int m_VAO, m_VBO, m_EBO;
};