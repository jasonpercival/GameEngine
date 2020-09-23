#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glew.h>

#include <vector>
#include "../FX/LightSource.h"
#include "../Math/Frustum.h"

class Camera
{
public:
	Camera();
	~Camera();

	void SetPosition(glm::vec3 position_);
	void SetRotation(float yaw_, float pitch_);

	glm::mat4 GetView() const;
	glm::mat4 GetPerspective() const;
	glm::mat4 GetOrthographic() const;
	glm::vec3 GetPosition() const;

	Frustum GetFrustum() const;
	glm::vec2 GetClippingPlanes() const;

	void AddLightSource(glm::vec3 position_, float ambience_, float diffuse_, glm::vec3 color_);
	std::vector<LightSource*> GetLightSources();

	void ProcessMouseMovement(glm::vec2 offset_);
	void ProcessMouseZoom(int y_);

private:
	void UpdateCameraVector();
	void OnDestroy();

	glm::vec3 position;
	glm::mat4 perspective, orthographic;
	float fieldOfView;
	float yaw, pitch;
	float nearPlane, farPlane;
	glm::vec3 forward, up, right, worldUp;

	std::vector<LightSource*> lights;
};
#endif // !CAMERA_H

