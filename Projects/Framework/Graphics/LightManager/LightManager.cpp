#include "FrameworkPCH.h"
#include "LightManager.h"

LightManager::LightManager()
	:Singleton<LightManager>()
{

}

LightManager::~LightManager()
{

}

void LightManager::AddLight(Light light)
{
	for (unsigned int i = 0; i < 10; i++)
	{
		if (!m_Lights[i].bEnabled)
		{
			m_Lights[i] = light;
		}
	}
}

const std::array<Light, 10>& LightManager::GetLights() const
{
	return m_Lights;
}