#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Eigen/Dense>

#include "drawer/drawer.h"
#include "drawer/primitive.h"
#include "geometryProcess/cubicSpline.h"
#include "drawer/scene.h"
#include "drawer/light.h"
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
int main() {
		std::cout << "Hello, World!" << std::endl;

		Scene* scene = new Scene();

		Light* light = new Light(glm::vec3(1.2f, 1.0f, 2.0f), glm::vec3(1.0, 1.0, 1.0));

		scene->light = light;
		

		Drawer *drawer =new Drawer(scene);
		drawer->initWindows();
		glfwSetFramebufferSizeCallback(drawer->window, framebuffer_size_callback);
		// 添加灯光物体
		Primitive* lightCube = new Primitive();
		lightCube->shaderName = "basicColorShader";
		lightCube->modelMatrix = glm::translate(lightCube->modelMatrix, glm::vec3(1.2f, 1.0f, 2.0f));
		lightCube->modelMatrix = glm::scale(lightCube->modelMatrix, glm::vec3(0.2f));
		scene->primitives.push_back(lightCube);
		lightCube->setup();
		// 添加几何体
		Primitive* cubeObj = new Primitive();
		cubeObj->shaderName = "phongColorShader";
		cubeObj->baseColor = glm::vec4(1.0, 1.0, 0.0, 1.0);
		cubeObj->setup();
		scene->primitives.push_back(cubeObj);

		drawer->draw();

		system("pause");
		return 0;
}