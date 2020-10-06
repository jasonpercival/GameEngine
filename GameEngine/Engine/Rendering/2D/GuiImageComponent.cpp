#include "GuiImageComponent.h"

GuiImageComponent::GuiImageComponent() : spriteSurface(nullptr)
{
}

GuiImageComponent::~GuiImageComponent()
{
	if (spriteSurface)
	{
		delete spriteSurface, spriteSurface = nullptr;
	}
}

bool GuiImageComponent::OnCreate(const std::string& imageName_, glm::vec2 offset_, glm::vec2 scale_, float angle_, glm::vec4 tint_)
{
	spriteSurface = new SpriteSurface(imageName_, scale_, angle_, tint_);
	offset = offset_;
	return true;
}

void GuiImageComponent::Draw(Camera* camera_, glm::vec2 position_)
{
	spriteSurface->Draw(camera_, position_ + offset);
}

bool GuiImageComponent::FindContainingPoint(glm::vec2 position_)
{
	// check if position is within bounds of the sprite surface
	float width = spriteSurface->GetWidth() / 2.0f;
	float height = spriteSurface->GetHeight() / 2.0f;
	return (position_.x >= offset.x - width && position_.x <= offset.x + width) &&
		(position_.y <= -offset.y + height && position_.y >= -offset.y - height);
}
