#ifndef OPENGLMESH_H
#define OPENGLMESH_H

#include <glew.h>
#include "Mesh.h"

class OpenGLMesh : public Mesh
{
public:
	OpenGLMesh(SubMesh subMesh_, GLuint shaderProgram_);
	~OpenGLMesh();
	
	// Inherited via Mesh
	void Render(Camera* camera_, std::vector<BoundingBox> instances_) override;

private:
	void GenerateBuffers();

	SubMesh subMesh;

	GLuint VAO, VBO;
	GLuint shaderProgram;
	GLuint modelLoc, viewLoc, projLoc;

	// Light locations
	GLuint viewPositionLoc, lightPosLoc, lightambientLoc, lightdiffuseLoc, lightColorLoc;

	// Material locations
	GLuint diffuseMapLoc, shininessLoc, transparencyLoc, ambientLoc, diffuseLoc, specularLoc;
};

#endif
