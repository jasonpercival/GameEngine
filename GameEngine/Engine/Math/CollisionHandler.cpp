#include "CollisionHandler.h"
#include "../Core/CoreEngine.h"

std::unique_ptr<CollisionHandler> CollisionHandler::collisionInstance = nullptr;
std::vector<GameObject*> CollisionHandler::prevCollisions = std::vector<GameObject*>();
OctSpatialPartition* CollisionHandler::scenePartition = nullptr;

CollisionHandler::CollisionHandler()
{
	prevCollisions.reserve(10);
}

CollisionHandler::~CollisionHandler()
{
	OnDestroy();
}

CollisionHandler* CollisionHandler::GetInstance()
{
	if (collisionInstance.get() == nullptr)
	{
		collisionInstance.reset(new CollisionHandler);
	}
	return collisionInstance.get();
}

void CollisionHandler::OnCreate(float worldSize_)
{
	prevCollisions.clear();
	scenePartition = new OctSpatialPartition(worldSize_);
}

void CollisionHandler::AddObject(GameObject* go_)
{
	scenePartition->AddObject(go_);
}

void CollisionHandler::MouseUpdate(glm::vec2 mousePosition_, int buttonType_)
{
	// check gui objects first
	float sizeX = CoreEngine::GetInstance()->GetWindowSize().x / 2.0f;
	float sizeY = CoreEngine::GetInstance()->GetWindowSize().y / 2.0f;
	glm::vec2 mouseCoords = glm::vec2(mousePosition_.x - sizeX, sizeY - mousePosition_.y);

	GuiObject* guiObjectHit = nullptr;
	for (auto guiObject : SceneGraph::GetInstance()->sceneGuiObjects)
	{
		if (guiObject.second->FindContainingPoint(mouseCoords))
		{
			guiObjectHit = guiObject.second;
			std::cout << "UI object clicked: " << guiObjectHit->GetTag() << std::endl;
			return;
		}
	}

	// otherwise check game objects
	Ray mouseRay = CollisionDetection::ScreenPosToWorldRay(mousePosition_,
		CoreEngine::GetInstance()->GetWindowSize(),
		CoreEngine::GetInstance()->GetCamera());

	if (scenePartition != nullptr)
	{
		GameObject* hitResult = scenePartition->GetCollision(mouseRay);

		if (hitResult)
		{
			hitResult->SetHit(true, buttonType_);
		}

		for (auto c : prevCollisions)
		{
			if (c != hitResult && c != nullptr)
			{
				c->SetHit(false, buttonType_);
			}
			c = nullptr;
		}
		prevCollisions.clear();

		if (hitResult)
		{
			prevCollisions.push_back(hitResult);
		}
	}
}

void CollisionHandler::OnDestroy()
{
	delete scenePartition, scenePartition = nullptr;

	for (auto entry : prevCollisions)
	{
		entry = nullptr;
	}
	prevCollisions.clear();
}
