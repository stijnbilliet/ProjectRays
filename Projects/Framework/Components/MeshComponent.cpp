#include "FrameworkPCH.h"
#include "MeshComponent.h"
#include "Scenegraph/GameObject.h"
#include "Components/MeshComponent.h"
#include "Data/MeshData.h"
#include "Data/TextureData.h"
#include "Scenegraph/Scene.h"

using Super = SceneComponent;

MeshComponent::MeshComponent(std::shared_ptr<MeshData> pMeshData, std::shared_ptr<TextureData> pTextureData)
	:SceneComponent(), m_Color(1.0f, 0.0f, 1.0f, 1.0f), m_pMeshData(pMeshData), m_pTextureData(pTextureData)
{
}

MeshComponent::~MeshComponent()
{
}

void MeshComponent::OnDraw(Renderer* pContext) const
{
	Super::OnDraw(pContext);
}

void MeshComponent::SetMeshData(std::shared_ptr<MeshData> pNewData)
{
	//no cleanup required since smartpointer
	m_pMeshData = pNewData;
}

std::shared_ptr<MeshData> MeshComponent::GetMeshData()
{
	return m_pMeshData;
}

void MeshComponent::SetTextureData(std::shared_ptr<TextureData> pNewData)
{
	//no cleanup required since smartpointer
	m_pTextureData = pNewData;
}

std::shared_ptr<TextureData> MeshComponent::GetTextureData()
{
	return m_pTextureData;
}
