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
	float lastFrame = 0.0; //��һ֡��ʱ��
	float deltaFrame = 0.0; //��һ֡����һ֡��ʱ���
	// ��Ӱ��һЩ����
	std::vector<glm::mat4> shadowTransforms;

	// ���������
	bool mouseLeftEnter = false;
	float lastXpos = -10000.0, lastYpos = -10000.0;
	float yaw=-90, pitch=0.0;
	glm::vec3 cameraFront;

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