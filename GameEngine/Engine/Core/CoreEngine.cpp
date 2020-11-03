#include "CoreEngine.h"
#include "../Events/MouseEventListener.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl.h"
#include "backends/imgui_impl_opengl3.h"


std::unique_ptr<CoreEngine> CoreEngine::engineInstance = nullptr;

CoreEngine::CoreEngine() :
	window(nullptr), isRunning(false), fps(120), gameInterface(nullptr), currentSceneNum(0), camera(nullptr)
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

void CoreEngine::SetGameInterface(GameInterface* gameInterface_)
{
	gameInterface = gameInterface_;
}

int CoreEngine::GetCurrentScene()
{
	return currentSceneNum;
}

glm::vec2 CoreEngine::GetWindowSize() const
{
	return glm::vec2(window->GetWidth(), window->GetHeight());
}

Camera* CoreEngine::GetCamera() const
{
	return camera;
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

void CoreEngine::NotifyOfMousePressed(glm::vec2 mouse_)
{
}

void CoreEngine::NotifyOfMouseReleased(glm::vec2 mouse_, int buttonType_)
{
	CollisionHandler::GetInstance()->MouseUpdate(mouse_, buttonType_);
}

void CoreEngine::NotifyOfMouseMove(glm::vec2 mouse_)
{
	if (camera)
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

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

	window = new Window();
	if (!window->OnCreate(name_, width_, height_))
	{
		Debug::FatalError("Window failed to initialize.", "CoreEngine.cpp", __LINE__);
		return isRunning = false;
	}

	SDL_WarpMouseInWindow(window->GetWindow(), window->GetWidth() / 2, window->GetHeight() / 2);

	MouseEventListener::RegisterEngineObject(this);

	ShaderHandler::GetInstance()->CreateProgram("basicShader",
		"Engine/Shaders/VertexShader.glsl",
		"Engine/Shaders/FragmentShader.glsl");

	ShaderHandler::GetInstance()->CreateProgram("colorShader",
		"Engine/Shaders/ColorVertexShader.glsl",
		"Engine/Shaders/ColorFragmentShader.glsl");

	ShaderHandler::GetInstance()->CreateProgram("spriteShader",
		"Engine/Shaders/SpriteVertShader.glsl",
		"Engine/Shaders/SpriteFragShader.glsl");

	ShaderHandler::GetInstance()->CreateProgram("particleShader",
		"Engine/Shaders/particleVertShader.glsl",
		"Engine/Shaders/particleFragShader.glsl");

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
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window->GetWindow());
	ImGui::NewFrame();

	if (gameInterface)
	{
		gameInterface->Update(deltaTime_);
	}
}

void CoreEngine::Render()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (gameInterface)
	{
		gameInterface->Render();
		gameInterface->Draw();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_GL_SwapWindow(window->GetWindow());
}

void CoreEngine::OnDestroy()
{
	AudioHandler::GetInstance()->OnDestroy();
	ShaderHandler::GetInstance()->OnDestroy();
	TextureHandler::GetInstance()->OnDestroy();
	SceneGraph::GetInstance()->OnDestroy();
	CollisionHandler::GetInstance()->OnDestroy();

	delete camera;
	camera = nullptr;

	delete gameInterface;
	gameInterface = nullptr;

	delete window;
	window = nullptr;
	
	SDL_Quit();
	//exit(0);
}
