#include "FrameworkPCH.h"
#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> verts, std::vector<unsigned int> indices)
	:m_Vertices(verts), m_Indices(indices)
{
	glGenVertexArrays(1, &m_VAO); //generate arrays to store attribute pointers
	glGenBuffers(1, &m_VBO); //gen vertrex buffer
	glGenBuffers(1, &m_EBO); //gen element buffer

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(m_VAO);
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);

		//vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
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
