#include "LightSource.h"

LightSource::LightSource(glm::vec3 position_, float ambient_, float diffuse_, glm::vec3 color_)
{
	position = position_;
	ambient = ambient_;
	diffuse = diffuse_;
	color = color_;
}

LightSource::~LightSource()
{
}

glm::vec3 LightSource::GetPosition() const
{
	return position;
}

float LightSource::GetAmbient() const
{
	return ambient;
}

float LightSource::GetDiffuse() const
{
	return diffuse;
}

glm::vec3 LightSource::GetColor() const
{
	return color;
}
