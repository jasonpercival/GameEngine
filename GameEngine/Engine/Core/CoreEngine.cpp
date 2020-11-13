#include "CoreEngine.h"
#include "../Events/MouseEventListener.h"

std::unique_ptr<CoreEngine> CoreEngine::engineInstance = nullptr;

CoreEngine::CoreEngine() :
	window(nullptr), isRunning(false), fps(60), gameInterface(nullptr), currentSceneNum(0), camera(nullptr),
	renderer(nullptr), rendererType(RendererType::NONE), allowCameraMove(false)
{
}

CoreEngine::~CoreEngine()
{
	OnDestroy();
}

CoreEngine* CoreEngine::GetInstance()
{
	if (engineInstance.get() == nullptr)
	{
		engineInstance.reset(new CoreEngine);
	}
	return engineInstance.get();
}

void CoreEngine::SetGameInterface(GameInterface* gameInterface_, RendererType rendererType_)
{
	gameInterface = gameInterface_;
	rendererType = rendererType_;
}

int CoreEngine::GetCurrentScene()
{
	return currentSceneNum;
}

void CoreEngine::SetFPS(int fps_)
{
	fps = fps_;
}

glm::vec2 CoreEngine::GetWindowSize() const
{
	return glm::vec2(window->GetWidth(), window->GetHeight());
}

Camera* CoreEngine::GetCamera() const
{
	return camera;
}

Window* CoreEngine::GetWindow() const
{
	return window;
}

void CoreEngine::SetCurrentScene(int sceneNum_)
{
	currentSceneNum = sceneNum_;
}

void CoreEngine::SetCamera(Camera* camera_)
{
	camera = camera_;
}

void CoreEngine::Exit()
{
	isRunning = false;
}

void CoreEngine::NotifyOfMousePressed(glm::vec2 mouse_, int buttonType_)
{
	if (buttonType_ == 3)
		allowCameraMove = true;
}

void CoreEngine::NotifyOfMouseReleased(glm::vec2 mouse_, int buttonType_)
{
	CollisionHandler::GetInstance()->MouseUpdate(mouse_, buttonType_);
	if (buttonType_ == 3)
		allowCameraMove = false;

}

void CoreEngine::NotifyOfMouseMove(glm::vec2 mouse_)
{
	if (camera && allowCameraMove)
		camera->ProcessMouseMovement(MouseEventListener::GetMouseOffset());
}

void CoreEngine::NotifyOfMouseScroll(int y_)
{
	if (camera)
		camera->ProcessMouseZoom(y_);
}

bool CoreEngine::OnCreate(std::string name_, int width_, int height_)
{
	Debug::DebugInit();
	Debug::SetSeverity(MessageType::TYPE_INFO);

	// initialize renderer
	switch (rendererType)
	{
	case RendererType::OPENGL:
		renderer = new OpenGLRenderer();
		break;
	case RendererType::VULKAN:
		Debug::FatalError("Vulkan renderer is not available at this time.", "CoreEngine.cpp", __LINE__);
		return false;
	default:
		Debug::FatalError("Unsupported renderer.", "CoreEngine.cpp", __LINE__);
		return false;
	}

	// Imgui initialization
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

	// create window
	window = new Window();
	if (!window->OnCreate(name_, width_, height_, renderer))
	{
		Debug::FatalError("Window failed to initialize.", "CoreEngine.cpp", __LINE__);
		return isRunning = false;
	}

	SDL_WarpMouseInWindow(window->GetWindow(), window->GetWidth() / 2, window->GetHeight() / 2);

	MouseEventListener::RegisterEngineObject(this);

	if (gameInterface)
	{
		if (!gameInterface->OnCreate())
		{
			Debug::FatalError("Failed to create game interface", "CoreEngine.cpp", __LINE__);
			return isRunning = false;
		}
	}

	timer.Start();
	Debug::Info("Engine started", "CoreEngine.cpp", __LINE__);
	return isRunning = true;
}

void CoreEngine::Run()
{
	while (isRunning)
	{
		timer.UpdateFrameTicks();
		EventListener::Update();
		Update(timer.GetDeltaTime());
		Render();
		SDL_Delay(timer.GetSleepTime(fps));
	}

	if (!isRunning)	OnDestroy();
}

bool CoreEngine::GetIsRunning() const
{
	return isRunning;
}

void CoreEngine::Update(const float deltaTime_)
{
	if (gameInterface)
		gameInterface->Update(deltaTime_);
}

void CoreEngine::Render()
{
	if (gameInterface)
		gameInterface->Render();
}

void CoreEngine::OnDestroy()
{
	AudioHandler::GetInstance()->OnDestroy();
	ShaderHandler::GetInstance()->OnDestroy();
	TextureHandler::GetInstance()->OnDestroy();
	SceneGraph::GetInstance()->OnDestroy();
	CollisionHandler::GetInstance()->OnDestroy();

	delete camera, camera = nullptr;
	delete gameInterface, gameInterface = nullptr;
	delete renderer, renderer = nullptr;
	delete window, window = nullptr;

	SDL_Quit();
}
