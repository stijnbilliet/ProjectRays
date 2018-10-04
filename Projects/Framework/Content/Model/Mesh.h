#pragma once
#include "Patterns/BaseObject.h"

#include "Content/Model/Vertex.h"
#include "Content/Model/Texture.h"

class Mesh
{
public:
	Mesh(std::vector<Vertex> verts, std::vector<unsigned int> indices, std::vector<Texture> textures);

	virtual ~Mesh();

	unsigned int GetVertexArrayObj() const;
	unsigned int GetVertexBufferObj() const;
	unsigned int GetElementBufferObj() const;

	const std::vector<Vertex>& GetVertices() const;
	const std::vector<unsigned int>& GetIndices() const;
	const std::vector<Texture>& GetTextures() const;

	void SetTextures(const std::vector<Texture>& texture);

	bool ShouldDrawIndexed() const;
private:
	std::vector<Vertex> m_Vertices;
	std::vector<Texture> m_Textures;
	std::vector<unsigned int> m_Indices;

	bool m_bDrawIndexed;

	unsigned int m_VAO, m_VBO, m_EBO;
};