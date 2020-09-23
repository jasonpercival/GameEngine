
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Model.h"
#include "Component.h"
#include "/Dev/GameEngine/GameEngine/Game/Test/TestComponentA.h"

class GameObject
{
public:
	GameObject(Model* model_, glm::vec3 position_ = glm::vec3());
	~GameObject();
	void Render(Camera* camera_);
	void Update(const float deltaTime_);

	glm::vec3 GetPosition() const;
	float GetAngle() const;
	glm::vec3 GetRotation() const;
	glm::vec3 GetScale() const;

	void SetPosition(glm::vec3 position_);
	void SetAngle(float angle_);
	void SetRotation(glm::vec3 rotation_);
	void SetScale(glm::vec3 scale_);

	BoundingBox GetBoundingBox();
	std::string GetTag() const;
	void SetTag(std::string tag_);
	bool GetHit() const;
	void SetHit(bool hit_, int buttonType_);

	template<typename T>
	inline void AddComponent()
	{
		// create new component and validate type
		T* component = new T;
		if (dynamic_cast<Component*>(component) == nullptr)
		{
			Debug::Error("Only component types can be added to game objects.", __FILE__, __LINE__);
			delete component, component = nullptr;
			return;
		}

		// check to make sure component does not already exist
		if (GetComponent<T>() != nullptr)
		{
			Debug::Error("Unable to add component that already exists on a game object.", __FILE__, __LINE__);
			delete component, component = nullptr;
			return;
		}

		components.push_back(component);
		component->OnCreate(this);
	};

	template<typename T>
	inline T* GetComponent()
	{
		for (auto c : components)
		{
			if (dynamic_cast<T*>(c)) return (T*)c;
		}
		return nullptr;
	};

	template<typename T>
	inline void RemoveComponent()
	{
		for (size_t i = 0; i < components.size(); i++)
		{
			if (dynamic_cast<T*>(components[i]) != nullptr)
			{
				delete components[i];
				components.erase(components.begin() + i);
				break;
			}
		}
	};

private:
	Model* model;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	float angle;

	int modelInstance;
	BoundingBox box;

	std::string tag;
	bool hit;

	std::vector<Component*> components;

};

#endif // !GAMEOBJECT_H