#ifndef COREENGINE_H
#define COREENGINE_H

#include <memory>

#include "Window.h"
#include "Timer.h"
#include "Debug.h"
#include "GameInterface.h"
#include "Scene.h"

#include "../Math/Random.h"
#include "../Camera/Camera.h"
#include "../Graphics/ShaderHandler.h"
#include "../Graphics/TextureHandler.h"
#include "../Rendering/3D/GameObject.h"
#include "../Rendering/SceneGraph.h"
#include "../Rendering/3D/OpenGLRenderer.h"
#include "../Rendering/2D/GuiImageComponent.h"
#include "../FX/ParticleEmitter.h"
#include "../Events/EventListener.h"
#include "../Testing/TestComponentA.h"
#include "../Testing/TestComponentB.h"
#include "../Audio/AudioHandler.h"
#include "../Audio/AudioSource.h"

class CoreEngine
{
public:
	// disable copy/move constructors
	CoreEngine(const CoreEngine&) = delete;
	CoreEngine(CoreEngine&&) = delete;
	CoreEngine& operator=(const CoreEngine&) = delete;
	CoreEngine& operator=(CoreEngine&&) = delete;

	bool OnCreate(std::string name_, int width_, int height_);
	void Run();

	bool GetIsRunning() const;
	int GetCurrentScene();
	glm::vec2 GetWindowSize() const;
	Camera* GetCamera() const;
	Window* GetWindow() const;

	// singleton
	static CoreEngine* GetInstance();

	void SetGameInterface(GameInterface* gameInterface_, RendererType rendererType_ = RendererType::OPENGL);
	void SetCurrentScene(int sceneNum_);
	void SetCamera(Camera* camera_);

	void Exit();
	void NotifyOfMousePressed(glm::vec2 mouse_);
	void NotifyOfMouseReleased(glm::vec2 mouse_, int buttonType_);
	void NotifyOfMouseMove(glm::vec2 mouse_);
	void NotifyOfMouseScroll(int y_);

	inline RendererType GetRendererType() const { return rendererType; }
	inline Renderer* GetRenderer() const { return renderer; }

private:
	CoreEngine();
	~CoreEngine();

	static std::unique_ptr<CoreEngine> engineInstance;
	friend std::default_delete<CoreEngine>;

	void Update(const float deltaTime_);
	void Render();
	void OnDestroy();

	Camera* camera;
	Window* window;

	Timer timer;
	unsigned int fps;

	bool isRunning;

	GameInterface* gameInterface;
	int currentSceneNum;

	RendererType rendererType;
	Renderer* renderer;
};

#endif

