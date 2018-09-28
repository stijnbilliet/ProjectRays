#pragma once
#include "Components\BaseComponent.h"

class MeshData;
class TextureData;
class Renderer;
class MeshComponent final : public BaseComponent
{
public:
	MeshComponent(std::shared_ptr<MeshData> pMeshData, std::shared_ptr<TextureData> pTextureData = nullptr);
	virtual ~MeshComponent();

	MeshComponent(const MeshComponent& other) = delete;
	MeshComponent(MeshComponent&& other) = delete;
	MeshComponent& operator=(const MeshComponent& other) = delete;
	MeshComponent& operator=(MeshComponent&& other) = delete;

	void OnDraw(Renderer* pContext) const override;

	void SetMeshData(std::shared_ptr<MeshData> pNewData);
	std::shared_ptr<MeshData> GetMeshData();

	void SetTextureData(std::shared_ptr<TextureData> pNewData);
	std::shared_ptr<TextureData> GetTextureData();
private:
	glm::vec4 m_Color;
	std::shared_ptr<MeshData> m_pMeshData;
	std::shared_ptr<TextureData> m_pTextureData;
};