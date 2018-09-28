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

class Window;
class Renderer : public SingleInstance<Renderer>
{
public:
	Renderer();
	~Renderer();

	void Clear();
	void Present();

	virtual void OnInit() override;

	const static WindowSettings _WindowSettings;
private:
	SDL_GLContext m_Context;

	SDL_Window* m_pWindow;
	bool m_Vsync;
};