#include "FrameworkPCH.h"
#include "Scenegraph/GameObject.h"
#include "Components/MeshDrawComponent.h"
#include "Scenegraph/Scene.h"
#include "Content/Model/Mesh.h"
#include "Content/Shader/ShaderProgram.h"
#include "Components/TransformComponent.h"
#include "Components/CameraComponent.h"

using Super = BaseComponent;

MeshDrawComponent::MeshDrawComponent(const Mesh* pMeshData, ShaderProgram* pShaderProgram)
	:BaseComponent(), m_pMeshData(pMeshData), m_pShaderProgram(pShaderProgram)
{
}

MeshDrawComponent::~MeshDrawComponent()
{
	safe_delete(m_pShaderProgram);
}

void MeshDrawComponent::OnDraw(Renderer* pContext) const
{
	Super::OnDraw(pContext);
	
	m_pShaderProgram->Use();

	//send variables to shader
	auto pScene = (Scene*)m_pAttachedTo->GetScene();

	auto pWorldMat = m_pAttachedTo->GetTransform()->GetWorldMatrix();

	//worldvar
	int worldMatVar = glGetUniformLocation(m_pShaderProgram->GetId(), "gMatrixWorld");
	glUniformMatrix4fv(worldMatVar, 1, GL_FALSE, glm::value_ptr(pWorldMat));

	//worldviewprojection
	int wvpMatVar = glGetUniformLocation(m_pShaderProgram->GetId(), "gMatrixWVP");
	auto pWvpMat = pScene->GetActiveCamera()->GetViewProjection() * pWorldMat;
	glUniformMatrix4fv(wvpMatVar, 1, GL_FALSE, glm::value_ptr(pWvpMat));

	//Get textures from meshdata
	std::vector<Texture> textures = m_pMeshData->GetTextures();

	unsigned int diffuseNmb = 1;
	unsigned int specularNmb = 1;
	unsigned int opacityNmb = 1;
	unsigned int normalNmb = 1;
	unsigned int ambientNmb = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); //active proper texture unit
		std::string number;
		std::string name;
		switch (textures[i].type)
		{
			case TextureType::DIFFUSE:
				number = std::to_string(diffuseNmb++);
				name = "texture_diffuse";
				break;
			case TextureType::SPECULAR:
				number = std::to_string(specularNmb++);
				name = "texture_specular";
				break;
			case TextureType::OPACITY:
				number = std::to_string(opacityNmb++);
				name = "texture_opacity";
				break;
			case TextureType::NORMAL:
				number = std::to_string(normalNmb++);
				name = "texture_normal";
				break;
			case TextureType::AMBIENT:
				number = std::to_string(ambientNmb++);
				name = "texture_ambient";
				break;
			default:
				break;
		}

		std::string textureName = name + number;
		int textureloc = glGetUniformLocation(m_pShaderProgram->GetId(), textureName.c_str());
		glUniform1i(textureloc, i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);

	// draw mesh
	glBindVertexArray(m_pMeshData->GetVertexArrayObj());
	{
		if (m_pMeshData->ShouldDrawIndexed())
		{
			glDrawElements(GL_TRIANGLES, GLsizei(m_pMeshData->GetIndices().size()), GL_UNSIGNED_INT, 0);
		}
		else
		{
			glDrawArrays(GL_TRIANGLES, 0, GLsizei(m_pMeshData->GetVertices().size()));
		}
	}
	glBindVertexArray(0);
}