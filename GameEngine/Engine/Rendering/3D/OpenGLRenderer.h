#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include <gl/glew.h>
#include <SDL_opengl.h>
#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_opengl3.h>

#include "Renderer.h"
#include "../../Core/Debug.h"

class OpenGLRenderer : public Renderer
{
public:
	OpenGLRenderer();
	~OpenGLRenderer();

	// Inherited via Renderer
	virtual SDL_Window* CreateWindow(const std::string& name_, int width_, int height_) override;
	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Render() override;

private:
	void SetPreAttributes();
	void SetPostAttributes();

	SDL_GLContext context;
};

#endif
