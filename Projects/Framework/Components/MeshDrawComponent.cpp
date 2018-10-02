#include "FrameworkPCH.h"
#include "Scenegraph/GameObject.h"
#include "Components/MeshDrawComponent.h"
#include "Scenegraph/Scene.h"
#include "Content/Model/Mesh.h"

using Super = BaseComponent;

MeshDrawComponent::MeshDrawComponent(Mesh* pMeshData)
	:BaseComponent(), m_pMeshData(pMeshData)
{
}

MeshDrawComponent::~MeshDrawComponent()
{
	safe_delete(m_pMeshData);
}

void MeshDrawComponent::OnDraw(Renderer* pContext) const
{
	Super::OnDraw(pContext);
	
	// draw mesh
	glBindVertexArray(m_pMeshData->GetVertexArrayObj());
	glDrawElements(GL_TRIANGLES, GLsizei(m_pMeshData->GetIndices().size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}