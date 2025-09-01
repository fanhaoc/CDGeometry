#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Drawer
{
public:
	Drawer() {};
	~Drawer() {};

	GLFWwindow* window;

	int initWindows();
	int draw();
	void processInput(GLFWwindow* window);
private:

};