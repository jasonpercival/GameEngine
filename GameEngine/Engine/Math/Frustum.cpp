#include "Frustum.h"

Frustum::Frustum(glm::mat4 view_, glm::mat4 perspective_)
{
	// M = matrix	
	glm::mat4 M = perspective_ * view_;

	// Planes: 
	// [0] Near		= M4 + M3
	// [1] Far		= M4 - M3
	// [2] Left		= M4 + M1
	// [3] Right	= M4 - M1
	// [4] Bottom	= M4 + M2
	// [5] Top		= M4 - M2

	// Near plane 
	plane[0].x = M[0][3] + M[0][2];
	plane[0].y = M[1][3] + M[1][2];
	plane[0].z = M[2][3] + M[2][2];
	plane[0].w = M[3][3] + M[3][2];

	// Far Plane 
	plane[1].x = M[0][3] - M[0][2];
	plane[1].y = M[1][3] - M[1][2];
	plane[1].z = M[2][3] - M[2][2];
	plane[1].w = M[3][3] - M[3][2];

	// Left Plane
	plane[2].x = M[0][3] + M[0][0];
	plane[2].y = M[1][3] + M[1][0];
	plane[2].z = M[2][3] + M[2][0];
	plane[2].w = M[3][3] + M[3][0];

	// Right Plane
	plane[3].x = M[0][3] - M[0][0];
	plane[3].y = M[1][3] - M[1][0];
	plane[3].z = M[2][3] - M[2][0];
	plane[3].w = M[3][3] - M[3][0];

	// Bottom Plane
	plane[4].x = M[0][3] + M[0][1];
	plane[4].y = M[1][3] + M[1][1];
	plane[4].z = M[2][3] + M[2][1];
	plane[4].w = M[3][3] + M[3][1];

	// Top Plane
	plane[5].x = M[0][3] - M[0][1];
	plane[5].y = M[1][3] - M[1][1];
	plane[5].z = M[2][3] - M[2][1];
	plane[5].w = M[3][3] - M[3][1];

	// Normalize all the planes
	for (int i = 0; i < 6; i++)
	{
		NormalizePlane(plane[i]);
	}
}

void Frustum::NormalizePlane(glm::vec4& plane)
{
	float mag = sqrt(plane.x * plane.x + plane.y * plane.y + plane.z * plane.z);
	plane.x = plane.x / mag;
	plane.y = plane.y / mag;
	plane.z = plane.z / mag;
	plane.w = plane.w / mag;
}

// check if a point is inside the view frustum
bool Frustum::IsPointInFrustum(glm::vec3 point_)
{
	for (int i = 0; i < 6; i++)
	{
		glm::vec3 normal = glm::vec3(plane[i].x, plane[i].y, plane[i].z);
		float dist = glm::dot(normal, point_) + plane[i].w;
		if (dist < 0.0f)
		{
			return false;
		}
	}
	return true;
}

// check if all OBB vertices are inside the view frustum
bool Frustum::IsBoxInFrustum(BoundingBox box)
{
	// transform all 8 box points first
	glm::vec3 point[8];
	point[0] = glm::vec3(box.transform * glm::vec4(box.minVert.x, box.minVert.y, box.minVert.z, 1.0f));
	point[1] = glm::vec3(box.transform * glm::vec4(box.maxVert.x, box.minVert.y, box.minVert.z, 1.0f));
	point[2] = glm::vec3(box.transform * glm::vec4(box.minVert.x, box.maxVert.y, box.minVert.z, 1.0f));
	point[3] = glm::vec3(box.transform * glm::vec4(box.maxVert.x, box.maxVert.y, box.minVert.z, 1.0f));
	point[4] = glm::vec3(box.transform * glm::vec4(box.minVert.x, box.minVert.y, box.maxVert.z, 1.0f));
	point[5] = glm::vec3(box.transform * glm::vec4(box.maxVert.x, box.minVert.y, box.maxVert.z, 1.0f));
	point[6] = glm::vec3(box.transform * glm::vec4(box.minVert.x, box.maxVert.y, box.maxVert.z, 1.0f));
	point[7] = glm::vec3(box.transform * glm::vec4(box.maxVert.x, box.maxVert.y, box.maxVert.z, 1.0f));

	// check each plane
	for (int i = 0; i < 6; i++)
	{
		glm::vec3 normal = glm::vec3(plane[i].x, plane[i].y, plane[i].z);

		// make sure all the points are on the correct side of the plane
		int count = 0;
		count += ((glm::dot(normal, point[0]) + plane[i].w) < 0.0f ? 1 : 0);
		count += ((glm::dot(normal, point[1]) + plane[i].w) < 0.0f ? 1 : 0);
		count += ((glm::dot(normal, point[2]) + plane[i].w) < 0.0f ? 1 : 0);
		count += ((glm::dot(normal, point[3]) + plane[i].w) < 0.0f ? 1 : 0);
		count += ((glm::dot(normal, point[4]) + plane[i].w) < 0.0f ? 1 : 0);
		count += ((glm::dot(normal, point[5]) + plane[i].w) < 0.0f ? 1 : 0);
		count += ((glm::dot(normal, point[6]) + plane[i].w) < 0.0f ? 1 : 0);
		count += ((glm::dot(normal, point[7]) + plane[i].w) < 0.0f ? 1 : 0);

		if (count == 8) return false;
	}

	return true;
}