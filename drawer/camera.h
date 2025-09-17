#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Camera
{
public:
	glm::vec3 cameraPos;
	glm::vec3 cameraTarget;
	glm::vec3 cameraFront;
	glm::vec3 cameraDirection;
	glm::vec3 worldUp;
	glm::vec3 cameraRight;
	glm::vec3 cameraUp;
	glm::mat4 viewMatrix;
	Camera();
	~Camera();
	void updateCameraParams();
	void moveCameraPos(glm::vec3 direction, float distance);

private:

};
