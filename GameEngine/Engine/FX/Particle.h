#ifndef PARTICLE_H
#define PARTICLE_H

#include <gl/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Camera/Camera.h"
#include "../Rendering/3D/Mesh.h"
#include "../Math/Random.h"

class ParticleEmitter;

class Particle
{
	friend class ParticleEmitter;

public:
	Particle(GLuint shaderID_, GLuint textureID_, ParticleEmitter* particleEmitter_);
	~Particle();
	void Render(Camera* camera);

private:
	void GenerateBuffers();
	void Initialize();

	ParticleEmitter* pe;

	GLuint shaderID, textureID;
	glm::vec3 position, velocity, accel;
	float lifeTime;
	glm::vec3 color;
	float particleSize;

	GLuint VAO, VBO;
	std::vector<Vertex2D> vertexList;
	GLuint modelLoc, viewLoc, projLoc, tintColourLoc, inputTextureLoc;

};

#endif // !PARTICLE_H
