#ifndef GUIIMAGECOMPONENT_H
#define GUIIMAGECOMPONENT_H

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <string>
#include "GuiComponent.h"
#include "SpriteSurface.h"

class GuiImageComponent : public GuiComponent
{
public:
	GuiImageComponent();
	~GuiImageComponent();

	bool OnCreate(const std::string& imageName_, glm::vec2 offset_, glm::vec2 scale_ = glm::vec2(1.0f, 1.0f),
		float angle_ = 0.0f, glm::vec4 tint_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	void Draw(Camera* camera_, glm::vec2 position_) override;
	bool FindContainingPoint(glm::vec2 position_) override;
private:
	SpriteSurface* spriteSurface;
	glm::vec2 offset;

};

#endif

