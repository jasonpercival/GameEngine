#ifndef FRUSTUM_H
#define FRUSTUM_H

#include <glm/glm.hpp>
#include "BoundingBox.h"

class Frustum
{
public:
	Frustum(glm::mat4 view_, glm::mat4 perspective_);
	bool IsPointInFrustum(glm::vec3 point_);
	bool IsBoxInFrustum(BoundingBox box);

private:
	void NormalizePlane(glm::vec4& plane);
	glm::vec4 plane[6];
};

#endif

