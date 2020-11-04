#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>
#include <string>

#include "Debug.h"
#include "../Rendering/3D/Renderer.h"

class Window
{
public:
	Window();
	~Window();

	bool OnCreate(std::string name_, int width_, int height_, Renderer* renderer);
	void OnDestroy();

	int GetWidth();
	int GetHeight();
	SDL_Window* GetWindow() const;
private:
	int width, height;
	SDL_Window* window;
};

#endif




