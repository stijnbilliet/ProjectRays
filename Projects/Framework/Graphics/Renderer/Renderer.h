#pragma once
#include "Patterns/SingleInstance.h"

struct WindowSettings
{
	WindowSettings(const unsigned int width, const unsigned int height)
		:Width(width), Height(height)
	{

	};

	const unsigned int Width;
	const unsigned int Height;
};

class Renderer : public SingleInstance<Renderer>
{
public:
	Renderer();
	virtual ~Renderer();

	//TODO: Renderer should draw instead of game

	void Begin();
	void End();

	virtual void OnInit() override;

	const static WindowSettings _WindowSettings;
private:
	SDL_GLContext m_Context;
	SDL_Window* m_pWindow;
	bool m_Vsync;
};