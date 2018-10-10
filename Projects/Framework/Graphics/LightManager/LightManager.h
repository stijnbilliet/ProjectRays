#pragma once
#include "Patterns/Singleton.h"

enum LightType {
	Point,
	Spot
};

struct Light {
	glm::vec3 Direction;
	glm::vec3 Position;
	glm::vec3 Color;
	float Intensity;
	float Range;
	float SpotlightAngle;
	LightType Type;

	bool bEnabled;
};

class LightManager : public Singleton<LightManager>
{
public:
	LightManager();
	virtual ~LightManager();

	void AddLight(Light light);
	const std::array<Light, 10>& GetLights() const;

private:
	std::array<Light, 10> m_Lights;
};