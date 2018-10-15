#pragma once
#include "Components\BaseComponent.h"

class GL_Renderer;
class Mesh;
class ShaderProgram;
class Texture;
class GameContext;
class MeshDrawComponent final : public BaseComponent
{
public:
	MeshDrawComponent(const Mesh* pMeshData, ShaderProgram* pShaderProgram);
	virtual ~MeshDrawComponent();

	MeshDrawComponent(const MeshDrawComponent& other) = delete;
	MeshDrawComponent(MeshDrawComponent&& other) = delete;
	MeshDrawComponent& operator=(const MeshDrawComponent& other) = delete;
	MeshDrawComponent& operator=(MeshDrawComponent&& other) = delete;

	virtual void OnDraw(GameContext* pContext) const override;

	const Mesh* GetMeshData() const;
private:
	const Mesh* m_pMeshData;
	ShaderProgram* m_pShaderProgram;
};