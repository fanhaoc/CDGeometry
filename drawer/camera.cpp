#include "camera.h"

Camera::Camera()
{
	cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
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
	cameraDirection = glm::vec3(0.0, 0.0, -1.0);
	cameraRight = glm::normalize(glm::cross(worldUp, cameraDirection));
	cameraUp = glm::cross(cameraDirection, cameraRight);
	cameraTarget = cameraPos + cameraFront;
	viewMatrix = glm::lookAt(cameraPos, cameraTarget, cameraUp);
}

void Camera::moveCameraPos(glm::vec3 direction, float distance) {

}