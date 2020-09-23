#include "Window.h"

Window::Window() : width(0), height(0), window(nullptr)
{
}

Window::~Window()
{
	OnDestroy();
}

bool Window::OnCreate(std::string name_, int width_, int height_)
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		Debug::FatalError("Failed to initialize SDL", "Window.cpp", __LINE__);
		return false;
	}

	width = width_;
	height = height_;

	// Create Window
	SetPreAttributes();
	window = SDL_CreateWindow(name_.c_str(),
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height, SDL_WINDOW_OPENGL);

	if (!window)
	{
		Debug::FatalError("Failed to create window", "Window.cpp", __LINE__);
		return false;
	}

	// Create OpenGL Context
	context = SDL_GL_CreateContext(window);
	SetPostAttributes();

	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		Debug::FatalError("Failed to initialize GLEW", "Window.cpp", __LINE__);
		return false;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	glViewport(0, 0, width, height);
	
	// Retrieve vendor and OpenGL version
	std::string gl_vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
	std::string gl_version = reinterpret_cast<const char*>(glGetString(GL_VERSION));

	Debug::Info("Graphics Card: " + gl_vendor, "Window.cpp", __LINE__);
	Debug::Info("OpenGL version: " + gl_version, "Window.cpp", __LINE__);

	return true;
}

void Window::OnDestroy()
{
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	window = nullptr;
}

int Window::GetWidth()
{
	return width;
}

int Window::GetHeight()
{
	return height;
}

SDL_Window* Window::GetWindow() const
{
	return window;
}

void Window::SetPreAttributes()
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

void Window::SetPostAttributes()
{
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
}


