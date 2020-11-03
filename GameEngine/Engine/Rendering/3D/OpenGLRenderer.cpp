#include "OpenGLRenderer.h"

OpenGLRenderer::OpenGLRenderer() : context(nullptr)
{
}

OpenGLRenderer::~OpenGLRenderer()
{
	OnDestroy();
}

void OpenGLRenderer::SetPreAttributes()
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetSwapInterval(1); // sync with vertical retrace

	// set MSAA buffers and samples (anti-aliasing)
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

	glewExperimental = GL_TRUE;
}

void OpenGLRenderer::SetPostAttributes()
{
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
}

SDL_Window* OpenGLRenderer::CreateWindow(const std::string& name_, int width_, int height_)
{
	// Create Window
	SetPreAttributes();

	SDL_Window* window = SDL_CreateWindow(name_.c_str(),
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width_, height_, SDL_WINDOW_OPENGL);

	if (!window)
	{
		Debug::FatalError("Failed to create window", "OpenGLRenderer.cpp", __LINE__);
		return nullptr;
	}

	// Create OpenGL Context
	context = SDL_GL_CreateContext(window);
	SetPostAttributes();

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(window, context);
	ImGui_ImplOpenGL3_Init();

	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		Debug::FatalError("Failed to initialize GLEW", "OpenGLRenderer.cpp", __LINE__);
		OnDestroy();
		return nullptr;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	glViewport(0, 0, width_, height_);

	// Retrieve vendor and OpenGL version
	std::string gl_vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
	std::string gl_version = reinterpret_cast<const char*>(glGetString(GL_VERSION));

	Debug::Info("Graphics Card: " + gl_vendor, "OpenGLRenderer.cpp", __LINE__);
	Debug::Info("OpenGL version: " + gl_version, "OpenGLRenderer.cpp", __LINE__);

	return window;
}

bool OpenGLRenderer::OnCreate()
{
	return true;
}

void OpenGLRenderer::OnDestroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	SDL_GL_DeleteContext(context);
}

void OpenGLRenderer::Render()
{
}
