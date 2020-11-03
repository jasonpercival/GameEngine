#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>
#include <string>

enum class RendererType { NONE = 0, OPENGL, VULKAN };

class Renderer
{
public:

	Renderer() {};
	virtual ~Renderer() {};
	virtual SDL_Window* CreateWindow(const std::string& name_, int width_, int height_) = 0;
	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void Render() = 0;
};

#endif
