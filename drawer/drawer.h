#pragma once
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "primitive.h"
#include "camera.h"
class Drawer
{
public:
	Drawer() {};
	~Drawer() {};

	GLFWwindow* window;
	float screenWidth = 800;
	float screenHeight = 600;
	//glm::mat4 viewMatrix;
	//glm::mat4 projMatrix;
	std::vector<Primitive*> primitives;
	Camera* camera = new Camera();


	int initWindows();
	int draw();
	void processInput(GLFWwindow* window);
private:

};