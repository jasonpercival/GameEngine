#include "Model.h"
#include "OpenGLMesh.h"
#include "../../Core/CoreEngine.h"

Model::Model(const std::string& objFilePath_, const std::string& mtlFilePath_, GLuint shaderProgram_) :
	subMeshes(std::vector<Mesh*>()), shaderProgram(0), modelInstances(std::vector<BoundingBox>())
{
	subMeshes.reserve(10);
	shaderProgram = shaderProgram_;
	modelInstances.reserve(5);
	rendererType = CoreEngine::GetInstance()->GetRendererType();

	obj = new LoadOBJModel();
	obj->LoadModel(objFilePath_, mtlFilePath_);
	this->LoadModel();
}

Model::~Model()
{
	if (subMeshes.size() > 0)
	{
		for (auto m : subMeshes)
		{
			delete m;
			m = nullptr;
		}
	}

	if (modelInstances.size() > 0)
	{
		modelInstances.clear();
	}
}

void Model::Render(Camera* camera_)
{
	for (auto m : subMeshes)
	{
		m->Render(camera_, modelInstances);
	}
}

void Model::AddMesh(Mesh* mesh_)
{
	subMeshes.push_back(mesh_);
}

int Model::CreateInstance(glm::vec3 position_, float angle_, glm::vec3 rotation_, glm::vec3 scale_)
{
	box.transform = GetTransform(position_, angle_, rotation_, scale_);
	modelInstances.push_back(box);
	return (int)modelInstances.size() - 1;
}

void Model::UpdateInstance(int index_, glm::vec3 position_, float angle_, glm::vec3 rotation_, glm::vec3 scale_)
{
	modelInstances[index_].transform = GetTransform(position_, angle_, rotation_, scale_);
}

glm::mat4 Model::GetTransform(int index_) const
{
	return modelInstances[index_].transform;
}

BoundingBox Model::GetBoundingBox()
{
	return box;
}

GLuint Model::GetShaderProgram() const
{
	return shaderProgram;
}

glm::mat4 Model::GetTransform(glm::vec3 position_, float angle_, glm::vec3 rotation_, glm::vec3 scale_) const
{
	glm::mat4 model;
	model = glm::translate(model, position_);
	model = glm::rotate(model, angle_, rotation_);
	model = glm::scale(model, scale_);
	return model;
}

void Model::LoadModel()
{
	for (size_t i = 0; i < obj->GetSubMeshes().size(); i++)
	{
		if (rendererType == RendererType::OPENGL)
		{
			subMeshes.push_back(new OpenGLMesh(obj->GetSubMeshes()[i], shaderProgram));
		}
		else
		{
			Debug::FatalError("Unsupported RendererType", "Model.cpp", __LINE__);
			break;
		}
	}

	box = obj->GetBoundingBox();

	delete obj;
	obj = nullptr;
}