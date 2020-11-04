#ifndef MESH_H
#define MESH_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../Graphics/MaterialHandler.h"
#include "../../Camera/Camera.h"
#include "../../Math/BoundingBox.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 color;
};

struct Vertex2D
{
	glm::vec2 position;
	glm::vec2 texCoords;
};

struct SubMesh
{
	std::vector<Vertex> vertexList;
	std::vector<int> meshIndices;
	Material material;
};

class Mesh
{
public:
	Mesh() {};
	virtual ~Mesh() {};
	virtual void Render(Camera* camera_, std::vector<BoundingBox> instances_) = 0;
};

#endif

