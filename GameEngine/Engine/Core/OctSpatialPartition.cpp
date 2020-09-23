#include "OctSpatialPartition.h"

int OctNode::childNum = 0;

OctNode::OctNode(glm::vec3 position_, float size_, OctNode* parent_) :
	octBounds(nullptr), parent(nullptr), children(), objectList(std::vector<GameObject*>())
{
	objectList.reserve(10);
	octBounds = new BoundingBox();
	octBounds->minVert = position_;
	octBounds->maxVert = position_ + size_;
	size = size_;
	parent = parent_;

	for (size_t i = 0; i < CHILDREN_NUMBER; i++)
	{
		children[i] = nullptr;
	}
}

OctNode::~OctNode()
{
	OnDestroy();
}

void OctNode::OnDestroy()
{
	delete octBounds, octBounds = nullptr;

	if (objectList.size() > 0)
	{
		for (auto obj : objectList)
		{
			obj = nullptr;
		}
		objectList.clear();
	}
	for (int i = 0; i < CHILDREN_NUMBER; i++)
	{
		if (children[i] != nullptr)
		{
			delete children[i];
		}
		children[i] = nullptr;
	}
}

// create all of the children for the tree
void OctNode::Octify(int depth_)
{
	if (depth_ > 0 && this)
	{
		float half = size / 2.0f;

		children[static_cast<int>(OctChildren::OCT_TLF)] = new OctNode(
			glm::vec3(octBounds->minVert.x, octBounds->minVert.y + half, octBounds->minVert.z + half), half, this);

		children[static_cast<int>(OctChildren::OCT_BLF)] = new OctNode(
			glm::vec3(octBounds->minVert.x, octBounds->minVert.y, octBounds->minVert.z + half), half, this);

		children[static_cast<int>(OctChildren::OCT_BRF)] = new OctNode(
			glm::vec3(octBounds->minVert.x + half, octBounds->minVert.y, octBounds->minVert.z + half), half, this);

		children[static_cast<int>(OctChildren::OCT_TRF)] = new OctNode(
			glm::vec3(octBounds->minVert.x + half, octBounds->minVert.y + half, octBounds->minVert.z + half), half, this);

		children[static_cast<int>(OctChildren::OCT_TLR)] = new OctNode(
			glm::vec3(octBounds->minVert.x, octBounds->minVert.y + half, octBounds->minVert.z), half, this);

		children[static_cast<int>(OctChildren::OCT_BLR)] = new OctNode(octBounds->minVert, half, this);

		children[static_cast<int>(OctChildren::OCT_BRR)] = new OctNode(
			glm::vec3(octBounds->minVert.x + half, octBounds->minVert.y, octBounds->minVert.z), half, this);

		children[static_cast<int>(OctChildren::OCT_TRR)] = new OctNode(
			glm::vec3(octBounds->minVert.x + half, octBounds->minVert.y + half, octBounds->minVert.z), half, this);

		childNum += 8;
	}

	if (depth_ > 0 && this)
	{
		for (int i = 0; i < CHILDREN_NUMBER; i++)
		{
			children[i]->Octify(depth_ - 1);
		}
	}
}

OctNode* OctNode::GetParent()
{
	return parent;
}

OctNode* OctNode::GetChild(OctChildren childPos_)
{
	return children[static_cast<int>(childPos_)];
}

void OctNode::AddCollisionObject(GameObject* obj_)
{
	objectList.push_back(obj_);
}

int OctNode::GetObjectCount() const
{
	return objectList.size();
}

bool OctNode::IsLeaf() const
{
	return (children[0] == nullptr);
}

BoundingBox* OctNode::GetBoundingBox() const
{
	return octBounds;
}

int OctNode::GetChildCount() const
{
	return childNum;
}

OctSpatialPartition::OctSpatialPartition(float worldSize_) :
	root(nullptr), rayIntersectionLists(std::vector<OctNode*>())
{
	root = new OctNode(glm::vec3(-(worldSize_ / 2.0f)), worldSize_, nullptr);
	root->Octify(3);
	std::cout << "There are " << root->GetChildCount() << " child nodes" << std::endl;

	rayIntersectionLists.reserve(20);
}

OctSpatialPartition::~OctSpatialPartition()
{
	OnDestroy();
}

void OctSpatialPartition::OnDestroy()
{
	if (rayIntersectionLists.size() > 0)
	{
		for (auto cell : rayIntersectionLists)
		{
			cell = nullptr;
		}
		rayIntersectionLists.clear();
	}

	delete root, root = nullptr;
}

void OctSpatialPartition::AddObject(GameObject* obj_)
{
	AddObjectToCell(root, obj_);
}

GameObject* OctSpatialPartition::GetCollision(Ray ray_)
{

	if (rayIntersectionLists.size() > 0)
	{
		for (auto cell : rayIntersectionLists)
		{
			cell = nullptr;
		}
		rayIntersectionLists.clear();
	}

	PrepareCollisionQuery(root, ray_);

	GameObject* result = nullptr;
	float shortestDistance = FLT_MAX;
	for (auto cell : rayIntersectionLists)
	{
		for (auto obj : cell->objectList)
		{
			if (ray_.IsColliding(&obj->GetBoundingBox()))
			{
				if (ray_.intersectionDist < shortestDistance)
				{
					result = obj;
					shortestDistance = ray_.intersectionDist;
				}
			}
		}

		if (result != nullptr) return result;
	}

	return nullptr;
}

void OctSpatialPartition::AddObjectToCell(OctNode* cell_, GameObject* obj_)
{
	if (cell_)
	{
		BoundingBox cellBox = *cell_->GetBoundingBox();
		if (cellBox.Intersects(&obj_->GetBoundingBox()))
		{
			if (cell_->IsLeaf())
			{
				cell_->AddCollisionObject(obj_);
				std::cout << "Added " << obj_->GetTag() << " to cell: " << glm::to_string(cellBox.maxVert) << std::endl;
			}
			else
			{
				for (int i = 0; i < 8; i++)
				{
					AddObjectToCell(cell_->GetChild(static_cast<OctChildren>(i)), obj_);
				}
			}
		}
	}
}

void OctSpatialPartition::PrepareCollisionQuery(OctNode* cell_, Ray ray_)
{
	if (cell_)
	{
		BoundingBox cellBox = *cell_->GetBoundingBox();
		if (ray_.IsColliding(&cellBox))
		{
			if (cell_->IsLeaf())
			{
				rayIntersectionLists.push_back(cell_);
			}
			else
			{
				for (int i = 0; i < 8; i++)
				{
					PrepareCollisionQuery(cell_->GetChild(static_cast<OctChildren>(i)), ray_);
				}
			}
		}
	}
}
