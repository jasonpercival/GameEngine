#include "GuiObject.h"

GuiObject::GuiObject(glm::vec2 position_) : position(position_)
{
}

GuiObject::~GuiObject()
{
	// delete all components
	if (components.size() > 0)
	{
		for (auto c : components)
		{
			delete c, c = nullptr;
		}
		components.clear();
	}
}

void GuiObject::Draw(Camera* camera_)
{
	for (auto component : components)
	{
		component->Draw(camera_, position);
	}
}

std::string GuiObject::GetTag() const
{
	return tag;
}

void GuiObject::SetTag(std::string tag_)
{
	tag = tag_;
}

bool GuiObject::FindContainingPoint(glm::vec2 position_)
{
	for (auto c : components)
	{
		if (c->FindContainingPoint(position_)) return true;
	}
	return false;
}
