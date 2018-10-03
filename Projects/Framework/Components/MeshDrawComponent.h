#pragma once
#include "Components\BaseComponent.h"

class Renderer;
class Mesh;
class ShaderProgram;
class MeshDrawComponent final : public BaseComponent
{
public:
	MeshDrawComponent(Mesh* pMeshData, ShaderProgram* pShaderProgram);
	virtual ~MeshDrawComponent();

	MeshDrawComponent(const MeshDrawComponent& other) = delete;
	MeshDrawComponent(MeshDrawComponent&& other) = delete;
	MeshDrawComponent& operator=(const MeshDrawComponent& other) = delete;
	MeshDrawComponent& operator=(MeshDrawComponent&& other) = delete;

	virtual void OnDraw(Renderer* pContext) const override;
private:
	Mesh* m_pMeshData;
	ShaderProgram* m_pShaderProgram;
};