#include "SpriteSurface.h"

SpriteSurface::SpriteSurface(const std::string& imageName_, glm::vec2 scale_, float angle_, glm::vec4 tintColour_) :
	scale(scale_), angle(angle_), tintColour(tintColour_)
{
	// Get the gui shader program
	shaderProgram = ShaderHandler::GetInstance()->GetShader("spriteShader");

	// Generate plane for the surface
	vertexList = {
		{{ -0.5f,  0.5f}, { 0.0f, 0.0f }},
		{{  0.5f,  0.5f}, { 1.0f, 0.0f }},
		{{ -0.5f, -0.5f}, { 0.0f, 1.0f }},
		{{  0.5f, -0.5f}, { 1.0f, 1.0f }},
	};

	GenerateBuffers();

	// Load texture
	texture = TextureHandler::GetInstance()->GetTextureData(imageName_);
	if (texture == nullptr)
	{
		TextureHandler::GetInstance()->CreateTexture(imageName_,
			"./Resources/Textures/" + imageName_ + ".png");
		texture = TextureHandler::GetInstance()->GetTextureData(imageName_);
	}
}

SpriteSurface::~SpriteSurface()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	vertexList.clear();
	texture = nullptr;
}

void SpriteSurface::Draw(Camera* camera_, glm::vec2 position_)
{
	// image texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->textureID);
	glBindVertexArray(VAO);

	// orthographic projection matrix
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera_->GetOrthographic()));

	// model transform matrix
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(position_, 0.0f));
	model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(texture->width * scale.x, texture->height * scale.y, 1.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	// tint colour
	glUniform4fv(tintColourLoc, 1, glm::value_ptr(tintColour));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei) vertexList.size());

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void SpriteSurface::GenerateBuffers()
{
	// generate VAO and VBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexList.size() * sizeof(Vertex2D), &vertexList[0], GL_STATIC_DRAW);

	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (GLvoid*)0);

	// texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (GLvoid*)offsetof(Vertex2D, texCoords));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	modelLoc = glGetUniformLocation(shaderProgram, "model");
	projLoc = glGetUniformLocation(shaderProgram, "proj");

	tintColourLoc = glGetUniformLocation(shaderProgram, "tintColour");
	inputTextureLoc = glGetUniformLocation(shaderProgram, "inputTexture");
}
