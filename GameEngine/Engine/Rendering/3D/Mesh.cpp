#include "Mesh.h"

Mesh::Mesh(SubMesh subMesh_, GLuint shaderProgram_) :
	VAO(0), VBO(0), modelLoc(0), viewLoc(0), projLoc(0), viewPositionLoc(0), lightPosLoc(0), 
	lightambientLoc(0), lightdiffuseLoc(0), lightColorLoc(0), diffuseMapLoc(0), shininessLoc(0), 
	transparencyLoc(0), ambientLoc(0), diffuseLoc(0), specularLoc(0)
{
	shaderProgram = shaderProgram_;
	subMesh = subMesh_;
	GenerateBuffers();
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	if (subMesh.vertexList.size() > 0)
	{
		subMesh.vertexList.clear();
	}

	if (subMesh.meshIndices.size() > 0)
	{
		subMesh.meshIndices.clear();
	}
}

void Mesh::Render(Camera* camera_, std::vector<BoundingBox> instances_)
{
	glUniform1i(diffuseMapLoc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, subMesh.material.diffuseMap);

	// material properties	
	glUniform1f(shininessLoc, subMesh.material.shininess);
	glUniform1f(transparencyLoc, subMesh.material.transparency);
	glUniform3fv(ambientLoc, 1, glm::value_ptr(subMesh.material.ambient));
	glUniform3fv(diffuseLoc, 1, glm::value_ptr(subMesh.material.diffuse));
	glUniform3fv(specularLoc, 1, glm::value_ptr(subMesh.material.specular));

	// light source 
	LightSource* light = camera_->GetLightSources().front();
	glUniform3fv(viewPositionLoc, 1, glm::value_ptr(camera_->GetPosition()));
	glUniform3fv(lightPosLoc, 1, glm::value_ptr(light->GetPosition()));
	glUniform1f(lightambientLoc, light->GetAmbient());
	glUniform1f(lightdiffuseLoc, light->GetDiffuse());
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(light->GetColor()));

	// view and projection matrices
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera_->GetView()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera_->GetPerspective()));

	// get the camera frustum
	Frustum frustum = camera_->GetFrustum();

	// loop through all model matrices
	glBindVertexArray(VAO);

	int noRenderCount = 0;
	for (size_t i = 0; i < instances_.size(); i++)
	{
		// frustum culling using OBB
		if (frustum.IsBoxInFrustum(instances_[i]))
		{
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(instances_[i].transform));
			glDrawArrays(GL_TRIANGLES, 0, subMesh.vertexList.size());
		}
		else
		{
			noRenderCount++; // for testing
		}
	}

	// output culled objects for testing
	//if (noRenderCount > 0)
	//{
	//	std::cout << "Objects culled: " << noRenderCount << " of " << instances_.size() << std::endl;
	//}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Mesh::GenerateBuffers()
{
	// generate VAO and VBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, subMesh.vertexList.size() * sizeof(Vertex), &subMesh.vertexList[0], GL_STATIC_DRAW);

	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

	// normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));

	// texture coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoords));

	// color
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	modelLoc = glGetUniformLocation(shaderProgram, "model");
	viewLoc = glGetUniformLocation(shaderProgram, "view");
	projLoc = glGetUniformLocation(shaderProgram, "proj");

	viewPositionLoc = glGetUniformLocation(shaderProgram, "viewPosition");

	lightPosLoc = glGetUniformLocation(shaderProgram, "light.lightPos");
	lightambientLoc = glGetUniformLocation(shaderProgram, "light.ambient");
	lightdiffuseLoc = glGetUniformLocation(shaderProgram, "light.diffuse");
	lightColorLoc = glGetUniformLocation(shaderProgram, "light.lightColor");

	diffuseMapLoc = glGetUniformLocation(shaderProgram, "material.diffuseMap");
	shininessLoc = glGetUniformLocation(shaderProgram, "material.shininess");
	transparencyLoc = glGetUniformLocation(shaderProgram, "material.transparency");
	ambientLoc = glGetUniformLocation(shaderProgram, "material.ambient");
	diffuseLoc = glGetUniformLocation(shaderProgram, "material.diffuse");
	specularLoc = glGetUniformLocation(shaderProgram, "material.specular");
}
