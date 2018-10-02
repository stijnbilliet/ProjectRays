#include "FrameworkPCH.h"
#include "BasePawn.h"
#include "Components/MeshDrawComponent.h"
#include "Scenegraph/Scene.h"

BasePawn::BasePawn()
	:m_pController()
{
}

BasePawn::~BasePawn()
{
}

void BasePawn::SetPossessor(BaseController * pController)
{
	m_pController = pController;
}

BaseController* BasePawn::GetController() const
{
	return m_pController;
}
