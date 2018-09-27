#pragma once
#include "Scenegraph/Scene.h"

class Renderer;
class Canvas : public BasePanel
{
public:
	Canvas(const std::string& name);
	virtual ~Canvas();

	void OnInit() override;
	void OnUpdate(float deltaTime) override;
	void OnDraw(Renderer* pContext) const override;
};