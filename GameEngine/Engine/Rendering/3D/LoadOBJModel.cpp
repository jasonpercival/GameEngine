#include "LoadOBJModel.h"

LoadOBJModel::LoadOBJModel()
{
	currentMaterial = Material();
	vertices.reserve(200);
	normals.reserve(200);
	textureCoords.reserve(200);
	indices.reserve(200);
	normalIndices.reserve(200);
	textureIndices.reserve(200);
	meshVertices.reserve(200);
	subMeshes.reserve(10);
}

LoadOBJModel::~LoadOBJModel()
{
	OnDestroy();
}

std::vector<Vertex> LoadOBJModel::GetVerts()
{
	return meshVertices;
}

std::vector<int> LoadOBJModel::GetIndices()
{
	return indices;
}

std::vector<SubMesh> LoadOBJModel::GetSubMeshes()
{
	return subMeshes;
}

void LoadOBJModel::OnDestroy()
{
	vertices.clear();
	normals.clear();
	textureCoords.clear();
	indices.clear();
	normalIndices.clear();
	textureIndices.clear();
	meshVertices.clear();
	subMeshes.clear();
}

BoundingBox LoadOBJModel::GetBoundingBox() 
{
	return box;
}

void LoadOBJModel::LoadModel(const std::string& objFilePath_, const std::string& mtlFilePath_)
{
	LoadMaterialLibrary(mtlFilePath_);
	LoadModel(objFilePath_);
}

void LoadOBJModel::LoadModel(const std::string& filePath_)
{
	std::ifstream in(filePath_.c_str(), std::ios::in);
	if (!in)
	{
		Debug::Error("Cannot open OBJ file: " + filePath_, "LoadOBJModel.cpp", __LINE__);
		return;
	}

	// pre-initialize bounding box
	box.minVert = glm::vec3(100.0f);
	box.maxVert = glm::vec3(-100.0f);

	std::string line;
	while (std::getline(in, line))
	{
		std::string token = line.substr(0, 2);

		// VERTEX DATA
		if (token == "v ")
		{
			std::stringstream v(line.substr(2));
			float x, y, z;
			v >> x >> y >> z;
			vertices.push_back(glm::vec3(x, y, z));

			// find min vertex for bounding box
			if (x < box.minVert.x) box.minVert.x = x;
			if (y < box.minVert.y) box.minVert.y = y;
			if (z < box.minVert.z) box.minVert.z = z;

			// find max vertex for bounding box
			if (x > box.maxVert.x) box.maxVert.x = x;
			if (y > box.maxVert.y) box.maxVert.y = y;
			if (z > box.maxVert.z) box.maxVert.z = z;
		}
		// NORMAL DATA
		else if (token == "vn")
		{
			std::stringstream v(line.substr(2));
			float x, y, z;
			v >> x >> y >> z;
			normals.push_back(glm::vec3(x, y, z));
		}
		// TEXTURE DATA
		else if (token == "vt")
		{
			std::stringstream v(line.substr(2));
			float x, y;
			v >> x >> y;
			textureCoords.push_back(glm::vec2(x, 1.0f - y));
		}
		// FACE DATA
		else if (token == "f ")
		{
			std::stringstream v(line.substr(2));
			std::string tokenpart, value;

			for (int i = 0; i < 3; i++)
			{
				v >> tokenpart;

				// indices
				std::istringstream index(tokenpart);
				std::getline(index, value, '/');
				indices.push_back(std::stoi(value) - 1);

				// texture indices
				std::getline(index, value, '/');
				textureIndices.push_back(std::stoi(value) - 1);

				// normal indices
				std::getline(index, value, '/');
				normalIndices.push_back(std::stoi(value) - 1);
			}
		}
		// USE MATERIAL/NEW MESH 
		else if (line.substr(0, 7) == "usemtl ")
		{
			// process previous mesh first
			if (indices.size() > 0)
			{
				PostProcessing();
			}

			// use the new material
			LoadMaterial(line.substr(7));
		}
	}

	in.close();
	PostProcessing();
}

void LoadOBJModel::PostProcessing()
{
	for (size_t i = 0; i < indices.size(); i++)
	{
		Vertex vert;
		vert.position = vertices[indices[i]];
		vert.normal = normals[normalIndices[i]];
		vert.texCoords = textureCoords[textureIndices[i]];
		vert.color = glm::vec3(0.72f, 0.89f, 1.0f);
		meshVertices.push_back(vert);
	}
		
	SubMesh subMesh;
	subMesh.vertexList = meshVertices;
	subMesh.meshIndices = indices;
	subMesh.material = currentMaterial;
	subMeshes.push_back(subMesh);

	indices.clear();
	normalIndices.clear();
	textureIndices.clear();
	meshVertices.clear();

	currentMaterial = Material();
}

void LoadOBJModel::LoadMaterial(const std::string& matName_)
{
	currentMaterial = MaterialHandler::GetInstance()->GetMaterial(matName_);
}

void LoadOBJModel::LoadMaterialLibrary(const std::string& matFilePath_)
{
	MaterialLoader::LoadMaterial(matFilePath_);
}
