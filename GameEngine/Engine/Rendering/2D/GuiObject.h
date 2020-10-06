#ifndef GUIOBJECT_H
#define GUIOBJECT_H

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <iostream>

#include "..\..\Core\Debug.h"
#include "GuiComponent.h"

class Camera;

class GuiObject
{
public:
	GuiObject(glm::vec2 position_);
	~GuiObject();
	void Draw(Camera* camera_);

	std::string GetTag() const;
	void SetTag(std::string tag_);

	bool FindContainingPoint(glm::vec2 position_);

	template<typename T>
	inline void AddComponent()
	{
		// create new component and validate type
		T* component = new T;
		if (dynamic_cast<GuiComponent*>(component) == nullptr)
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
	glm::vec2 position;

	std::string tag;
	std::vector<GuiComponent*> components;
};

#endif