#include "FrameworkPCH.h"
#include "Renderer.h"

const WindowSettings Renderer::_WindowSettings = {1920, 1080};

Renderer::Renderer()
	:SingleInstance(), m_Context{}, m_pWindow{ nullptr }, m_Vsync(false)
{
}

Renderer::~Renderer()
{
	SDL_GL_DeleteContext(m_Context);
	SDL_DestroyWindow(m_pWindow);

	TTF_Quit();
	IMG_Quit();
}

void Renderer::Clear()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Present()
{
	SDL_GL_SwapWindow(m_pWindow);
}

void Renderer::OnInit()
{
	//Init SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	//Use openGL 2.1
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	//Create window
	m_pWindow = SDL_CreateWindow(
		"ProjectRays",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		_WindowSettings.Width,
		_WindowSettings.Height,
		SDL_WINDOW_OPENGL
	);
	if (m_pWindow == nullptr) throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());

	//Create openGL context
	m_Context = SDL_GL_CreateContext(m_pWindow);
	if (m_Context == nullptr) throw std::runtime_error(std::string("SDL_GL_CreateContext Error: ") + SDL_GetError());

	// synchronize it with the vertical retrace
	if (m_Vsync)
	{
		if (SDL_GL_SetSwapInterval(1) < 0)
		{
			std::cerr << "Renderer::Initialize( ), error when calling SDL_GL_SetSwapInterval: " << SDL_GetError() << std::endl;
			return;
		}
	}
	else
	{
		if (SDL_GL_SetSwapInterval(0) < 0)
		{
			std::cerr << "Renderer::Initialize( ), error when calling SDL_GL_SetSwapInterval: " << SDL_GetError() << std::endl;
			return;
		}
	}

	// Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		std::cerr << "Renderer::Initialize( ), error when calling IMG_Init: " << IMG_GetError() << std::endl;
		return;
	}

	// Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		std::cerr << "Renderer::Initialize( ), error when calling TTF_Init: " << TTF_GetError() << std::endl;
		return;
	}
}