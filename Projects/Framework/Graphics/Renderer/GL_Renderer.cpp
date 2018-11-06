#include "FrameworkPCH.h"
#include "GL_Renderer.h"

GL_Renderer::GL_Renderer()
	:SingleInstance(), m_Context{}, m_pWindow{ nullptr }, m_Vsync(false), m_ScrWidth(), m_ScrHeight(), m_gBuffer(), m_WindowTitle(), m_DirectionalPos(glm::vec3(0.0f, 80.0f, 5.0f)), m_DirectionalCol(glm::vec3(1.0f, 0.9f, 0.8f)), m_DirectionalSize(20.0f)
{
}

GL_Renderer::~GL_Renderer()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(m_Context);
	SDL_DestroyWindow(m_pWindow);
	safe_delete(m_pLightPass);
	safe_delete(m_pSimpleShader);
}

void GL_Renderer::Begin()
{
	//CLEAR COLOR
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//bind gbuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//render
	//...
}

void GL_Renderer::LightPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//2. lighting pass etc.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pLightPass->Use();

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, gLightAcc);

	//send light relevant information
	/*
	for (unsigned int i = 0; i < LightManager::GetInstance().GetLights().size(); i++)
	{
	Light light = m_Lights[i];
	std::string lightVarStr = "lights[" + std::to_string(i) + "]";

	unsigned int lDirectionVar = glGetUniformLocation(m_pLightPass->GetId(), (lightVarStr + ".Direction").c_str());
	unsigned int lPositionVar = glGetUniformLocation(m_pLightPass->GetId(), (lightVarStr + ".Position").c_str());
	unsigned int lColorVar = glGetUniformLocation(m_pLightPass->GetId(), (lightVarStr + ".Color").c_str());
	unsigned int lIntensityVar = glGetUniformLocation(m_pLightPass->GetId(), (lightVarStr + ".Intensity").c_str());
	unsigned int lRangeVar = glGetUniformLocation(m_pLightPass->GetId(), (lightVarStr + ".Range").c_str());
	unsigned int lSpotlightAngleVar = glGetUniformLocation(m_pLightPass->GetId(), (lightVarStr + ".SpotlightAngle").c_str());
	unsigned int lTypeVar = glGetUniformLocation(m_pLightPass->GetId(), (lightVarStr + ".Type").c_str());

	glUniform3fv(lDirectionVar, 1, &light.Direction[0]);
	glUniform3fv(lPositionVar, 1, &light.Position[0]);
	glUniform3fv(lColorVar, 1, &light.Color[0]);
	glUniform1f(lIntensityVar, light.Intensity);
	glUniform1f(lRangeVar, light.Range);
	glUniform1f(lSpotlightAngleVar, light.SpotlightAngle);
	glUniform1i(lTypeVar, int(light.Type));
	}
	*/

	//send directional light
	unsigned int directionalPosVar = glGetUniformLocation(m_pLightPass->GetId(), "dLightPos");
	glUniform3fv(directionalPosVar, 1, &m_DirectionalPos[0]);

	unsigned int directionalColVar = glGetUniformLocation(m_pLightPass->GetId(), "dLightCol");
	glUniform3fv(directionalColVar, 1, &m_DirectionalCol[0]);
}

void GL_Renderer::End()
{
	// finally render quad
	RenderQuad();

	// copy content of geometry's depth buffer to default framebuffer's depth buffer
	// -----------------------------------------------------------------------------
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	// write to default framebuffer
	// blit to default framebuffer. Note that this may or may not work as the internal formats of both the FBO and default framebuffer have to match.
	// the internal formats are implementation defined. This works on all of my systems, but if it doesn't on yours you'll likely have to write to the 		
	// depth buffer in another shader stage (or somehow see to match the default framebuffer's internal format with the FBO's internal format).
	glBlitFramebuffer(0, 0, m_ScrWidth, m_ScrHeight, 0, 0, m_ScrWidth, m_ScrHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//ImGUI
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_pWindow);
	ImGui::NewFrame();

	ImGuiOnDraw();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_GL_SwapWindow(m_pWindow);
}

const SDL_GLContext & GL_Renderer::GetGLContext() const
{
	return m_Context;
}

SDL_Window * GL_Renderer::GetWindow() const
{
	return m_pWindow;
}

unsigned int GL_Renderer::GetGBuffer() const
{
	return m_gBuffer;
}

unsigned int GL_Renderer::GetWorldPosBuffer() const
{
	return gPosition;
}

unsigned int GL_Renderer::GetNormalBuffer() const
{
	return gNormal;
}

unsigned int GL_Renderer::GetLightBuffer() const
{
	return gLightAcc;
}

unsigned int GL_Renderer::GetDepthBuffer() const
{
	return gDepth;
}

const glm::vec3 & GL_Renderer::GetDirectionalLightPos() const
{
	return m_DirectionalPos;
}

float GL_Renderer::GetDirectionalSize() const
{
	return m_DirectionalSize;
}

ShaderProgram * GL_Renderer::GetLightDrawer() const
{
	return m_pLightPass;
}

void GL_Renderer::OnInit(GameContext* pGameContext)
{
	UNREFERENCED_PARAMETER(pGameContext);

	PropertyManager::GetInstance().GetInt("renderer_viewwidth", m_ScrWidth);
	PropertyManager::GetInstance().GetInt("renderer_viewheight", m_ScrHeight);
	PropertyManager::GetInstance().GetString("windowtitle", m_WindowTitle);

	//Init SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	//Use openGL 3.3
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//Create window
	m_pWindow = SDL_CreateWindow(
		m_WindowTitle.c_str(),
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		m_ScrWidth,
		m_ScrHeight,
		SDL_WINDOW_OPENGL
	);
	if (m_pWindow == nullptr) throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());

	//Create openGL context
	m_Context = SDL_GL_CreateContext(m_pWindow);
	if (m_Context == nullptr) throw std::runtime_error(std::string("SDL_GL_CreateContext Error: ") + SDL_GetError());

	// synchronize it with the vertical retrace
	SDL_GL_SetSwapInterval(m_Vsync);

	//Ask for function bindings
	gladLoadGLLoader(SDL_GL_GetProcAddress);

	// Check OpenGL properties
	std::stringstream openGLInfo;
	openGLInfo << "OpenGL Specification Info\nVendor: " << glGetString(GL_VENDOR) << "\nRenderer: " << glGetString(GL_RENDERER) << "\nVersion: " << glGetString(GL_VERSION) << '\n';
	Logger::GetInstance().LogInfo(openGLInfo.str());

	//ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplSDL2_InitForOpenGL(m_pWindow, m_Context);
	ImGui_ImplOpenGL3_Init("#version 330");

	// Setup style
	ImGui::StyleColorsDark();
}

void GL_Renderer::PostInit(GameContext* pGameContext)
{
	UNREFERENCED_PARAMETER(pGameContext);

	//enable depth testing
	glEnable(GL_DEPTH_TEST);

	//setup g-buffers
	glGenFramebuffers(1, &m_gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);

	//gPosition
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_ScrWidth, m_ScrHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	//gAlbedo
	glGenTextures(1, &gAlbedo);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_ScrWidth, m_ScrHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gAlbedo, 0);

	//gNormal
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_ScrWidth, m_ScrHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gNormal, 0);

	//gLightAcc
	glGenTextures(1, &gLightAcc);
	glBindTexture(GL_TEXTURE_2D, gLightAcc);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_ScrWidth, m_ScrHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gLightAcc, 0);

	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
	unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, attachments);

	//create and attach depth buffer
	glGenRenderbuffers(1, &gDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, gDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, m_ScrWidth, m_ScrHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, gDepth);

	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer not complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// shader configuration
	// --------------------
	m_pLightPass = new ShaderProgram("Shaders/Deferred_Lightpass.vs", "Shaders/Deferred_Lightpass.fs");
	
	m_pLightPass->Use();

	int gPositionVar = glGetUniformLocation(m_pLightPass->GetId(), "gPosition");
	int gAlbedoVar = glGetUniformLocation(m_pLightPass->GetId(), "gAlbedo");
	int gNormalVar = glGetUniformLocation(m_pLightPass->GetId(), "gNormal");
	int gLightAccVar = glGetUniformLocation(m_pLightPass->GetId(), "gLightAcc");

	glUniform1i(gPositionVar, gPosition);
	glUniform1i(gAlbedoVar, gAlbedo);
	glUniform1i(gNormalVar, gNormal);
	glUniform1i(gLightAccVar, gLightAcc);
}

void GL_Renderer::RenderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};

		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void GL_Renderer::ImGuiOnDraw()
{
	ImGui::Begin("Debug");
		ImGui::Text("Average framerate (%.1f FPS)", ImGui::GetIO().Framerate);
	ImGui::End();

	ImGui::Begin("Light");
		ImGui::SliderFloat("Light Position x", &m_DirectionalPos.x, -20.0f, 20.0f);
		ImGui::SliderFloat("Light Position y", &m_DirectionalPos.y, 0.0f, 2000.0f);
		ImGui::SliderFloat("Light Position z", &m_DirectionalPos.z, -20.0f, 20.0f);
		ImGui::SliderFloat("Light Radius", &m_DirectionalSize, 0.0f, 200.0f);
		ImGui::ColorEdit3("Light Color", (float*)&m_DirectionalCol);
	ImGui::End();
}