#include "FrameworkPCH.h"
#include "Scenegraph/GameObject.h"
#include "Components/MeshDrawComponent.h"
#include "Scenegraph/Scene.h"
#include "Content/Model/Mesh.h"
#include "Content/Shader/ShaderProgram.h"
#include "Components/TransformComponent.h"
#include "Components/CameraComponent.h"

using Super = BaseComponent;

MeshDrawComponent::MeshDrawComponent(Mesh* pMeshData, ShaderProgram* pShaderProgram)
	:BaseComponent(), m_pMeshData(pMeshData), m_pShaderProgram(pShaderProgram)
{
}

MeshDrawComponent::~MeshDrawComponent()
{
	safe_delete(m_pMeshData);
	safe_delete(m_pShaderProgram);
}

void MeshDrawComponent::OnDraw(Renderer* pContext) const
{
	Super::OnDraw(pContext);
	
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

	// draw mesh
	m_pShaderProgram->Use();
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