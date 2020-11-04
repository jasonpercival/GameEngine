#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter(int particleCount_, const std::string& textureName_, const std::string& shaderName_) :
	particleCount(particleCount_), shaderID(0), textureID(0), particles(std::vector<Particle*>(particleCount_))
{
	// Load the shader program
	shaderID = ShaderHandler::GetInstance()->GetShader(shaderName_);

	// Load texture
	GLuint textureID = TextureHandler::GetInstance()->GetTexture(textureName_);
	if (textureID == 0)
	{
		TextureHandler::GetInstance()->CreateTexture(textureName_, "./Resources/Textures/Particles/" + textureName_ + ".png");
		textureID = TextureHandler::GetInstance()->GetTexture(textureName_);
	}

	// Create all the particles
	for (int i = 0; i < particleCount; i++)
	{
		particles[i] = new Particle(shaderID, textureID, this);
	}
}

ParticleEmitter::~ParticleEmitter()
{
	if (particles.size() > 0)
	{
		for (auto p : particles)
		{
			delete p, p = nullptr;
		}
		particles.clear();
	}
}

void ParticleEmitter::Update(float deltaTime)
{
	ImGui::Begin("Particle Emitter");
	ImGui::SliderInt("Particles", &particleCount, 1, 1000);	
	ImGui::SliderFloat3("Position", glm::value_ptr(startPosition), -10.0f, 10.0f, "%.3f");
	ImGui::SliderFloat("Gravity", &gravity, 0.0f, 20.0f);
	ImGui::SliderFloat3("Velocity Min", glm::value_ptr(velocityMin), -10.0f, 10.0f, "%.3f");
	ImGui::SliderFloat3("Velocity Max", glm::value_ptr(velocityMax), -10.0f, 10.0f, "%.3f");
	ImGui::SliderFloat("Max Size", &maxSize, 1.0f, 3.0f);
	ImGui::SliderFloat("Max Life Time", &maxLife, 0.1f, 10.0f);

	for (int i = 0; i < particleCount; i++)
	{
		if (particles[i] != nullptr)
		{
			// update life time
			particles[i]->lifeTime -= deltaTime;
			if (particles[i]->lifeTime <= 0.0f)
			{
				particles[i]->Initialize();
			}

			// update velocity
			particles[i]->velocity += particles[i]->accel * deltaTime;

			// update position
			particles[i]->position += particles[i]->velocity * deltaTime;
		}
	}

	ImGui::End();
}

void ParticleEmitter::Render(Camera* camera)
{
	glUseProgram(shaderID);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

	for (int i = 0; i < particleCount; i++)
	{
		particles[i]->Render(camera);
	}

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}



