#pragma once
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "primitive.h"
class Drawer
{
public:
	Drawer() {};
	~Drawer() {};

	GLFWwindow* window;
	std::vector<Primitive*> primitives;

	int initWindows();
	int draw();
	void processInput(GLFWwindow* window);
private:

};