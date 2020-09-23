#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include <glm/glm.hpp>

class LightSource
{
public:
	LightSource(glm::vec3 position_, float ambience_, float diffuse_, glm::vec3 color_);
	~LightSource();

	glm::vec3 GetPosition() const;
	float GetAmbient() const;
	float GetDiffuse() const;
	glm::vec3 GetColor() const;

private:
	glm::vec3 position;
	float ambient;
	float diffuse;
	glm::vec3 color;

};

#endif // !LIGHTSOURCE_H


