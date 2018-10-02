#include "FrameworkPCH.h"
#include "Renderer.h"
#include "Content/Shader/ShaderProgram.h"
#include "PropertyManager.h"
Renderer::Renderer()
	:SingleInstance(), m_Context{}, m_pWindow{ nullptr }, m_Vsync(true)
{
}

Renderer::~Renderer()
{
	SDL_GL_DeleteContext(m_Context);
	SDL_DestroyWindow(m_pWindow);

	delete m_pShaderProgram;
}

void Renderer::Begin()
{
	//CLEAR COLOR
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_pShaderProgram->Use();
}

void Renderer::End()
{
	SDL_GL_SwapWindow(m_pWindow);
}

void Renderer::OnInit()
{
	//Init SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	//Use openGL 3.3
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	int width{};
	int height{};
	std::string windowTitle{};
	PropertyManager::GetInstance().GetInt("renderer_viewwidth", width);
	PropertyManager::GetInstance().GetInt("renderer_viewheight", height);
	PropertyManager::GetInstance().GetString("windowtitle", windowTitle);
	
	//Create window
	m_pWindow = SDL_CreateWindow(
		windowTitle.c_str(),
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width,
		height,
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

	//Ask for function bindings
	gladLoadGLLoader(SDL_GL_GetProcAddress);

	// Check OpenGL properties
	printf("Vendor:   %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version:  %s\n", glGetString(GL_VERSION));
}

void Renderer::PostInit()
{
	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	
	//load shader from file
	std::string assetPath{};
	PropertyManager::GetInstance().GetString("assetpath", assetPath);
	std::string vshPathStr{ assetPath + "/Shaders/BasicShading.vsh" };
	std::string fshPathStr{ assetPath + "/Shaders/BasicShading.fsh" };
	const char* vshPath = vshPathStr.c_str();
	const char* fshPath = fshPathStr.c_str();

	m_pShaderProgram = new ShaderProgram(vshPath, fshPath);
	m_pShaderProgram->Use();
}