#include "EventListener.h"
#include "../Core/CoreEngine.h"
#include "MouseEventListener.h"

EventListener::~EventListener()
{

}

void EventListener::Update()
{
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent))
	{
		if (sdlEvent.type == SDL_QUIT)
		{
			CoreEngine::GetInstance()->Exit();
		}

		switch (sdlEvent.type)
		{
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEMOTION:
		case SDL_MOUSEWHEEL:
			MouseEventListener::Upate(sdlEvent);
			break;
		default:
			break;
		}
	}
}
