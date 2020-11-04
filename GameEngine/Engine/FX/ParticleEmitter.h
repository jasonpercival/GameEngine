#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include <string>
#include <glew.h>
#include <imgui.h>

#include "../Graphics/TextureHandler.h"
#include "../Graphics/ShaderHandler.h"
#include "../Math/Random.h"
#include "Particle.h"

class ParticleEmitter
{
public:
	ParticleEmitter(int particleCount_, const std::string& textureName_, const std::string& shaderName_);
	~ParticleEmitter();
	void Update(float deltaTime);
	void Render(Camera* camera);

	float GetMaxParticleSize() { return maxSize; }
	float GetMaxLife() { return maxLife; }
	float GetGravity() { return gravity; }
	glm::vec3 GetPosition() { return startPosition; }
	glm::vec3 GetVelocityMin() { return velocityMin; }
	glm::vec3 GetVelocityMax() { return velocityMax; }

private:
	int particleCount;
	GLuint textureID, shaderID;
	std::vector<Particle*> particles;

	// emitter properties
	float maxSize = 1.0f;
	float maxLife = 10.0f;
	float gravity = 9.8f;
	glm::vec3 startPosition = glm::vec3(0.0, 3.2f, 0.0f);
	glm::vec3 velocityMin = glm::vec3(-5.0, 0.0f, -3.0f);
	glm::vec3 velocityMax = glm::vec3(8.0, 8.0f, 8.0f);
};

#endif
