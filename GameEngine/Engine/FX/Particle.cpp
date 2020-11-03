#include "Particle.h"
#include "ParticleEmitter.h"

Particle::Particle(GLuint shaderID_, GLuint textureID_, ParticleEmitter* particleEmitter_)
	: shaderID(shaderID_), textureID(textureID_), lifeTime(1.0f), particleSize(1.0f), pe(particleEmitter_)
{
	// Generate plane for the particle
	vertexList = {
		{{ -0.5f,  0.5f}, { 0.0f, 0.0f }},
		{{  0.5f,  0.5f}, { 1.0f, 0.0f }},
		{{ -0.5f, -0.5f}, { 0.0f, 1.0f }},
		{{  0.5f, -0.5f}, { 1.0f, 1.0f }},
	};

	GenerateBuffers();
	Initialize();
}

Particle::~Particle()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	vertexList.clear();
}

// Initialize particle default values
void Particle::Initialize()
{
	lifeTime = Random::Range(1.0f, pe->GetMaxLife());
	accel = glm::vec3(0.0f, -pe->GetGravity(), 0.0f);
	velocity = glm::vec3(Random::Range(-5.0f, 8.0f), Random::Range(0.0f, 8.0f), Random::Range(-3.0f, 8.0f));
	position = pe->GetPosition();

	float color1 = Random::Range(0.8f, 1.0f);
	float color2 = Random::Range(0.1f, 1.0f);
	float extraRed = Random::Range(0.0f, 1.0f);
	color = glm::vec3(color1 + extraRed, color1 - extraRed, color2 - extraRed);
	particleSize = Random::Range(0.1f, pe->GetMaxParticleSize());
}

void Particle::Render(Camera* camera)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glBindVertexArray(VAO);

	// set view and projection matrices
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera->GetView()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera->GetPerspective()));

	// model transform matrix
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(position));
	//model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(particleSize));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	// colour
	glUniform4fv(tintColourLoc, 1, glm::value_ptr(color));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)vertexList.size());

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void Particle::GenerateBuffers()
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

	viewLoc = glGetUniformLocation(shaderID, "view");
	modelLoc = glGetUniformLocation(shaderID, "model");
	projLoc = glGetUniformLocation(shaderID, "proj");

	tintColourLoc = glGetUniformLocation(shaderID, "tintColour");
	inputTextureLoc = glGetUniformLocation(shaderID, "inputTexture");
}