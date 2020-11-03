#include "GameScene.h"

GameScene::GameScene() : Scene(), backgroundMusic(nullptr), particleEmitter(nullptr)
{
	Random::SetSeed();
}

GameScene::~GameScene()
{
	delete backgroundMusic, backgroundMusic = nullptr;
	delete particleEmitter, particleEmitter = nullptr;
	SceneGraph::GetInstance()->OnDestroy();
}

bool GameScene::OnCreate()
{
	Debug::Info("Creating game scene", "GameScene.cpp", __LINE__);	

	// setup the shaders
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

	// setup camera
	CoreEngine::GetInstance()->SetCamera(new Camera());
	CoreEngine::GetInstance()->GetCamera()->SetPosition(glm::vec3(0.0f, 0.0f, 20.0f));

	// setup audio
	AudioHandler::GetInstance()->Initialize(CoreEngine::GetInstance()->GetCamera()->GetPosition());

	// light sources
	CoreEngine::GetInstance()->GetCamera()->AddLightSource(glm::vec3(0.0f, 0.0f, 2.0f), 0.1f, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));

	// setup collision handler
	CollisionHandler::GetInstance()->OnCreate(100.0f);

	// create the apple model
	Model* appleModel = new Model("./Resources/Models/Apple.obj", "./Resources/Materials/Apple.mtl",
		ShaderHandler::GetInstance()->GetShader("basicShader"));
	SceneGraph::GetInstance()->AddModel(appleModel);

	// create an apple for testing mouse picking
	GameObject* apple = new GameObject(appleModel, glm::vec3(0.0f, 0.0f, -10.0f));

	// setup sound FX on a game object when clicked
	apple->AddComponent<AudioSource>("quack.wav", false, true, false);

	// add game object to the scene graph
	SceneGraph::GetInstance()->AddGameObject(apple, "Crab Apple");

	// create particle system
	particleEmitter = new ParticleEmitter(1000, "star", "particleShader");

	return true;
}

void GameScene::Update(const float deltaTime_)
{
	SceneGraph::GetInstance()->Update(deltaTime_);
	if (particleEmitter) particleEmitter->Update(deltaTime_);
}

void GameScene::Render()
{
	SceneGraph::GetInstance()->Render(CoreEngine::GetInstance()->GetCamera());
	if (particleEmitter) particleEmitter->Render(CoreEngine::GetInstance()->GetCamera());
}

void GameScene::Draw()
{
	SceneGraph::GetInstance()->Draw(CoreEngine::GetInstance()->GetCamera());
}
