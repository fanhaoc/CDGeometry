#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "primitive.h"
#include "camera.h"
#include "trick.h"

class Drawer
{
public:
	Drawer() {
		cameraFront = camera->cameraFront;
	};
	~Drawer() {};

	GLFWwindow* window;
	float screenWidth = 800;
	float screenHeight = 600;
	float lastFrame = 0.0; //上一帧的时间
	float deltaFrame = 0.0; //这一帧和上一帧的时间差

	// 摄像机参数
	bool mouseLeftEnter = false;
	float lastXpos = -10000.0, lastYpos = -10000.0;
	float yaw=-90, pitch=0.0;
	glm::vec3 cameraFront;

	//glm::mat4 viewMatrix;
	//glm::mat4 projMatrix;
	std::vector<Primitive*> primitives;
	Camera* camera = new Camera();


	int initWindows();
	int draw();
	void processInput(GLFWwindow* window);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
private:

};