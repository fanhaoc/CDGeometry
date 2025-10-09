#include "camera.h"

Camera::Camera()
{
	cameraPos = glm::vec3(3.0f, 1.5f, 5.0f);
	cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	cameraFront = glm::vec3(0.0, 0.0, -1.0);
	cameraDirection = glm::normalize(cameraPos - cameraTarget);
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	cameraRight = glm::normalize(glm::cross(worldUp, cameraDirection));
	cameraUp = glm::cross(cameraDirection, cameraRight);
	updateCameraParams();
}

Camera::~Camera()
{
}
void Camera::updateCameraParams() {
	//cameraDirection = glm::normalize(cameraPos - cameraTarget);
	// fps camera
	//cameraDirection = glm::vec3(0.0, 0.0, -1.0);
	cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
	cameraUp = glm::cross(cameraRight, cameraFront);
	cameraTarget = cameraPos + cameraFront;
	viewMatrix = glm::lookAt(cameraPos, cameraTarget, cameraUp);
}

void Camera::moveCameraPos(glm::vec3 direction, float distance) {
	float cameraSpeed = 2.5f * distance;
	cameraPos += direction * cameraSpeed;
	updateCameraParams();
}

glm::vec3 Camera::getForwardDir(float yaw) {
	float pitch = 0.0;
	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	return glm::normalize(front);
}