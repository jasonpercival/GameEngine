#include "Window.h"


Window::Window() : width(0), height(0), window(nullptr)
{
}

Window::~Window()
{
	OnDestroy();
}

bool Window::OnCreate(std::string name_, int width_, int height_, Renderer* renderer)
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		Debug::FatalError("Failed to initialize SDL", "Window.cpp", __LINE__);
		return false;
	}

	width = width_;
	height = height_;

	// Renderer creates specific type window
	window = renderer->CreateWindow(name_, width_, height_);
	return (window != nullptr);
}

void Window::OnDestroy()
{
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

