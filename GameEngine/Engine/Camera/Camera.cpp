#include "Camera.h"

#include "../Core/CoreEngine.h"

Camera::Camera() : position(glm::vec3())
{
	fieldOfView = 45.0f;
	forward = glm::vec3(0.0f, 0.0f, -1.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	worldUp = up;
	nearPlane = 2.0f;
	farPlane = 50.0f;
	yaw = -90.0f;
	pitch = 0.0f;

	perspective = glm::perspective(fieldOfView,
		CoreEngine::GetInstance()->GetWindowSize().x /
		CoreEngine::GetInstance()->GetWindowSize().y,
		nearPlane, farPlane);

	//orthographic = glm::ortho(
	//	0.0f, CoreEngine::GetInstance()->GetWindowSize().x,
	//	CoreEngine::GetInstance()->GetWindowSize().y, 0.0f,
	//	-1.0f, 1.0f);

	//orthographic = glm::ortho(
	//	0.0f, 
	//	CoreEngine::GetInstance()->GetWindowSize().x,
	//	0.0f, 
	//	CoreEngine::GetInstance()->GetWindowSize().y,
	//	-1.0f, 1.0f);

	orthographic = glm::ortho(
		-CoreEngine::GetInstance()->GetWindowSize().x / 2.0f,	// left
		CoreEngine::GetInstance()->GetWindowSize().x / 2.0f,	// right
		-CoreEngine::GetInstance()->GetWindowSize().y / 2.0f,	// bottom
		CoreEngine::GetInstance()->GetWindowSize().y / 2.0f,	// top
		-1.0f, 1.0f);											// near, far

	UpdateCameraVector();
}

Camera::~Camera()
{
	OnDestroy();
}

void Camera::SetPosition(glm::vec3 position_)
{
	position = position_;
	UpdateCameraVector();
}

void Camera::SetRotation(float yaw_, float pitch_)
{
	yaw = yaw_;
	pitch = pitch_;
	UpdateCameraVector();
}

glm::mat4 Camera::GetView() const
{
	return glm::lookAt(position, position + forward, up);
}

glm::mat4 Camera::GetPerspective() const
{
	return perspective;
}

glm::mat4 Camera::GetOrthographic() const
{
	return orthographic;
}

glm::vec3 Camera::GetPosition() const
{
	return position;
}

glm::vec2 Camera::GetClippingPlanes() const
{
	return glm::vec2(nearPlane, farPlane);
}

void Camera::AddLightSource(glm::vec3 position_, float ambience_, float diffuse_, glm::vec3 color_)
{
	LightSource* light = new LightSource(position_, ambience_, diffuse_, color_);
	lights.push_back(light);
}

std::vector<LightSource*> Camera::GetLightSources()
{
	return lights;
}

void Camera::ProcessMouseMovement(glm::vec2 offset_)
{
	offset_ *= 0.05f;

	yaw += offset_.x;
	pitch += offset_.y;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	if (yaw < 0.0f)
	{
		yaw += 360.0f;
	}
	if (yaw > 360.0f)
	{
		yaw -= 360.0f;
	}
	UpdateCameraVector();
}

void Camera::ProcessMouseZoom(int y_)
{
	if (y_ < 0 || y_ > 0)
	{
		position += static_cast<float>(y_) * (forward * 1.0f);
	}
	UpdateCameraVector();
}

void Camera::UpdateCameraVector()
{
	forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward.y = sin(glm::radians(pitch));
	forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	forward = glm::normalize(forward);
	right = glm::normalize(glm::cross(forward, worldUp));
	up = glm::normalize(glm::cross(right, forward));
}

Frustum Camera::GetFrustum() const
{
	return Frustum(GetView(), perspective);
}

void Camera::OnDestroy()
{
	for (auto l : lights)
	{
		delete l;
		l = nullptr;
	}
	lights.clear();
}
