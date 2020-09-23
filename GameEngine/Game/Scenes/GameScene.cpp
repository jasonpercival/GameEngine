#include "GameScene.h"
#include <chrono>

GameScene::GameScene() : Scene()
{
}

GameScene::~GameScene()
{
	SceneGraph::GetInstance()->OnDestroy();
}

bool GameScene::OnCreate()
{
	Debug::Info("Creating game scene", "GameScene.cpp", __LINE__);

	CoreEngine::GetInstance()->SetCamera(new Camera());
	CoreEngine::GetInstance()->GetCamera()->SetPosition(glm::vec3(0.0f, 0.0f, 20.0f));

	// light sources
	CoreEngine::GetInstance()->GetCamera()->AddLightSource(glm::vec3(0.0f, 0.0f, 2.0f), 0.1f, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));

	// setup collision handler
	CollisionHandler::GetInstance()->OnCreate(100.0f);

	// create the apple model
	Model* appleModel = new Model("./Resources/Models/Apple.obj", "./Resources/Materials/Apple.mtl",
		ShaderHandler::GetInstance()->GetShader("basicShader"));
	SceneGraph::GetInstance()->AddModel(appleModel);

	// create the apple model
	Model* diceModel = new Model("./Resources/Models/Dice.obj", "./Resources/Materials/Dice.mtl",
		ShaderHandler::GetInstance()->GetShader("basicShader"));
	SceneGraph::GetInstance()->AddModel(diceModel);

	// random number seed
	srand(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));

	// create some randomly placed dice
	for (int i = 0; i < 5; i++)
	{
		// random positions			
		float x = -5 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (12)));
		float y = -5 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (12)));
		float z = -5 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (12)));

		GameObject* dice = new GameObject(diceModel, glm::vec3(x, y, z));
		SceneGraph::GetInstance()->AddGameObject(dice, "Dice " + std::to_string(i));
	}

	// create an apple for testing mouse picking
	GameObject* apple = new GameObject(appleModel, glm::vec3(0.0f, 0.0f, -10.0f));

	// add test components to gameobject
	TestComponentA* testComponentA = new TestComponentA();
	TestComponentB* testComponentB = new TestComponentB();
	apple->AddComponent<TestComponentA>();
	apple->AddComponent<TestComponentB>();
	apple->AddComponent<TestComponentA>();
	apple->GetComponent<TestComponentB>();
	apple->RemoveComponent<TestComponentB>();

	// add game object to the scene graph
	SceneGraph::GetInstance()->AddGameObject(apple, "Crab Apple");
	return true;
}

void GameScene::Update(const float deltaTime_)
{
	SceneGraph::GetInstance()->Update(deltaTime_);
}

void GameScene::Render()
{
	SceneGraph::GetInstance()->Render(CoreEngine::GetInstance()->GetCamera());
}