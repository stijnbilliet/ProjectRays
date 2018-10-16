#pragma once
#include "Components\BaseComponent.h"

class Mesh;
class RayShapeComponent : public BaseComponent
{
public:
	RayShapeComponent(const Mesh* pMesh);
	virtual ~RayShapeComponent();

	void OnInit(GameContext* pContext) override;
	virtual void OnUpdate(GameContext* pContext) override;
	virtual void OnDraw(GameContext* pContext) const override;
private:

	static int _ShapeID;
	RadeonRays::Shape* m_pShape;
	const Mesh* m_pMeshData;
};