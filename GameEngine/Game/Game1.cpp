#include "Game1.h"

Game1::Game1() : GameInterface(), currentSceneNum(0), currentScene(nullptr)
{
}

Game1::~Game1()
{
	delete currentScene;
	currentScene = nullptr;
}

bool Game1::OnCreate()
{
	if (CoreEngine::GetInstance()->GetCurrentScene() == 0)
	{
		currentScene = new StartScene();
		currentSceneNum = 0;
		return currentScene->OnCreate();
	}

	Debug::FatalError("Engine's scene is not initialized to 0", "Game1.cpp", __LINE__);

	return false;
}

void Game1::Update(const float deltaTime_)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame((CoreEngine::GetInstance()->GetWindow()->GetWindow()));
	ImGui::NewFrame();

	if (currentSceneNum != CoreEngine::GetInstance()->GetCurrentScene())
	{
		BuildScene();
	}
	currentScene->Update(deltaTime_);
}

void Game1::Render()
{
	// clear screen
	RendererType renderType = CoreEngine::GetInstance()->GetRendererType();
	switch (renderType)
	{
	case RendererType::OPENGL:
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		break;
	default:
		break;
	}

	currentScene->Render();
	currentScene->Draw();

	// swap window
	switch (renderType)
	{
	case RendererType::OPENGL:
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(CoreEngine::GetInstance()->GetWindow()->GetWindow());
		break;
	default:
		break;
	}
}

void Game1::BuildScene()
{
	delete currentScene;
	currentScene = nullptr;

	// TODO: Change to scene enumeration
	switch (CoreEngine::GetInstance()->GetCurrentScene())
	{
	case 1:
		currentScene = new GameScene();
		break;
	default: // case 0: 
		currentScene = new StartScene();
		break;
	}

	currentSceneNum = CoreEngine::GetInstance()->GetCurrentScene();
	if (!currentScene->OnCreate())
	{
		Debug::FatalError("Unable to build scene", "Game1.cpp", __LINE__);
		CoreEngine::GetInstance()->Exit();
	}
}

