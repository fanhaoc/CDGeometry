#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "camera.h"
#include "trick.h"
#include "scene.h"
#include "geometry/model.h"
#include "primitives/superPrimitive.h"

class Drawer
{
public:
	Drawer(Scene *_scene) {
		scene = _scene;
		cameraFront = camera->cameraFront;
	};
	~Drawer() {};

	GLFWwindow* window;
	float screenWidth = 800;
	float screenHeight = 600;
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	float lastFrame = 0.0; //上一帧的时间
	float deltaFrame = 0.0; //这一帧和上一帧的时间差

	// 摄像机参数
	bool mouseLeftEnter = false;
	float lastXpos = -10000.0, lastYpos = -10000.0;
	float yaw=-90, pitch=0.0;
	glm::vec3 cameraFront;

	//glm::mat4 viewMatrix;
	//glm::mat4 projMatrix;
	std::vector<SuperPrimitive*> sps;
	Camera* camera = new Camera();

	Scene* scene;

	unsigned int UBO;

	int initWindows();
	int draw();
	void drawPrimitives(const unsigned int& fb, const unsigned int& shadowMap);
	void getFrameBuffer(unsigned int& framebuffer, unsigned int& fbTexture);
	void processInput(GLFWwindow* window);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
private:

};