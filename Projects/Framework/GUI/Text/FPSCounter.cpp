#include "FrameworkPCH.h"
#include "FPSCounter.h"
#include "Components/MeshComponent.h"
#include "Data/MeshData.h"
#include "Data/TextureData.h"
#include "Scenegraph/Scene.h"
#include "Graphics/Renderer.h"

using Super = TextLabel;

FPSCounter::FPSCounter(int ptSize, const glm::vec4& txtColor)
	:TextLabel(ptSize, txtColor), m_ElapsedSec()
{
}

FPSCounter::~FPSCounter()
{
}

void FPSCounter::PreInit()
{
	Super::PreInit();
}

void FPSCounter::OnInit()
{
	Super::OnInit();

	GetRootComponent()->Translate(glm::vec2(m_ptSize*2.0f, m_ptSize));
}

void FPSCounter::OnUpdate(float deltaTime)
{
	Super::OnUpdate(deltaTime);

	m_ElapsedSec += deltaTime;
	if (m_ElapsedSec > 1.0f)
	{
		unsigned int fps{ unsigned int(1.0f / deltaTime) };
		SetText(std::to_string(fps));
		m_ElapsedSec = 0.0f;
	}
}