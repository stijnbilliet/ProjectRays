#include "FrameworkPCH.h"
#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> verts, std::vector<unsigned int> indices, std::vector<Texture> textures)
	:m_Vertices(verts), m_Indices(indices), m_Textures(textures)
{
	m_Indices.size() > 0 ? m_bDrawIndexed = true : m_bDrawIndexed = false;

	glGenVertexArrays(1, &m_VAO); //generate arrays to store attribute pointers
	glGenBuffers(1, &m_VBO); //gen vertrex buffer
	m_bDrawIndexed ? glGenBuffers(1, &m_EBO) : 0;

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(m_VAO);
	{
		//vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);

		//element buffer
		if(m_bDrawIndexed)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);
		}

		//vertex attributes
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	}
	glBindVertexArray(0);
}

Mesh::~Mesh()
{
}

unsigned int Mesh::GetVertexArrayObj() const
{
	return m_VAO;
}

unsigned int Mesh::GetVertexBufferObj() const
{
	return m_VBO;
}

unsigned int Mesh::GetElementBufferObj() const
{
	return m_EBO;
}

const std::vector<Vertex>& Mesh::GetVertices() const
{
	return m_Vertices;
}

const std::vector<unsigned int>& Mesh::GetIndices() const
{
	return m_Indices;
}

const std::vector<Texture>& Mesh::GetTextures() const
{
	return m_Textures;
}

void Mesh::SetTextures(const std::vector<Texture>& texture)
{
	m_Textures = texture;
}

bool Mesh::ShouldDrawIndexed() const
{
	return m_bDrawIndexed;
}