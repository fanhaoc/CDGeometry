#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Eigen/Dense>

#include "drawer/drawer.h"
#include "drawer/primitive.h"
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
int main() {
		std::cout << "Hello, World!" << std::endl;
		Eigen::Matrix3d fixedMatrix;

		Drawer drawer;
		drawer.initWindows();
		glfwSetFramebufferSizeCallback(drawer.window, framebuffer_size_callback);
		// ��Ӽ�����
		drawer.primitives.push_back(new Primitive());

		drawer.draw();

		system("pause");
		return 0;
}