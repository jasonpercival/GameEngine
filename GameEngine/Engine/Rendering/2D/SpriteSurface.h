#ifndef SPRITESURFACE_H
#define SPRITESURFACE_H

#include <iostream>
#include <glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../Camera/Camera.h"
#include "../../Graphics/TextureHandler.h"
#include "../../Graphics/ShaderHandler.h"

struct Vertex2D
{
	glm::vec2 position;
	glm::vec2 texCoords;
};

class SpriteSurface
{
public:
	SpriteSurface(const std::string& imageName_, glm::vec2 scale_ = glm::vec2(1.0f, 1.0f), 
		float angle_ = 0.0f, glm::vec4 tintColour_ = glm::vec4(1, 1, 1, 1));

	~SpriteSurface();
	void Draw(Camera* camera_, glm::vec2 position_);

	inline float GetWidth() { return static_cast<float>(texture->width); }
	inline float GetHeight() { return static_cast<float>(texture->height); }
private:

	void GenerateBuffers();

	const Texture* texture;
	glm::vec2 scale;
	float angle;
	glm::vec4 tintColour;
	std::vector<Vertex2D> vertexList;

	GLuint VAO, VBO;
	GLuint shaderProgram;
	GLuint modelLoc, projLoc;
	GLuint tintColourLoc, inputTextureLoc;
};

#endif