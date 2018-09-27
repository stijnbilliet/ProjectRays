#include "FrameworkPCH.h"
#include "Canvas.h"
#include "Scenegraph/GameObject.h"

using Super = BasePanel;

Canvas::Canvas(const std::string& name)
	:BasePanel(name)
{
}

Canvas::~Canvas()
{
}

void Canvas::OnInit()
{
	Super::OnInit();

	for (auto uielement : m_pObjects)
	{
		uielement->Init();
	}
}

void Canvas::OnUpdate(float deltaTime)
{
	Super::OnUpdate(deltaTime);

	//Update gameObjects
	for (auto uielement : m_pObjects)
	{
		uielement->Update(deltaTime);
	}
}

void Canvas::OnDraw(Renderer * pContext) const
{
	Super::OnDraw(pContext);

	for (const auto uielement : m_pObjects)
	{
		uielement->Draw(pContext);
	}
}
