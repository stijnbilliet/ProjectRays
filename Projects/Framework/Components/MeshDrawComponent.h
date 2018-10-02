#pragma once
#include "Components\BaseComponent.h"

class Renderer;
class Mesh;
class MeshDrawComponent final : public BaseComponent
{
public:
	MeshDrawComponent(Mesh* pMeshData);
	virtual ~MeshDrawComponent();

	MeshDrawComponent(const MeshDrawComponent& other) = delete;
	MeshDrawComponent(MeshDrawComponent&& other) = delete;
	MeshDrawComponent& operator=(const MeshDrawComponent& other) = delete;
	MeshDrawComponent& operator=(MeshDrawComponent&& other) = delete;

	void OnDraw(Renderer* pContext) const override;
private:
	Mesh* m_pMeshData;
};