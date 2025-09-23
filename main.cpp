#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Eigen/Dense>
#include <assimp/Importer.hpp>
#include <Windows.h>

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

	// 设置 dll 搜索目录为 exe 目录下的 dlls 子目录
	//char buffer[MAX_PATH];
	//GetModuleFileName(NULL, buffer, MAX_PATH);
	//std::string exeDir = std::string(buffer);
	//exeDir = exeDir.substr(0, exeDir.find_last_of("\\/"));
	//exeDir += "\\bin";
	//std::cout << exeDir << std::endl;

	//SetDllDirectory(exeDir.c_str());

	Assimp::Importer importer;

	Scene* scene = new Scene();

	Light* light = new Light(glm::vec3(1.2f, 1.0f, 2.0f), glm::vec3(1.0, 1.0, 1.0));

	scene->light = light;
		

	Drawer *drawer =new Drawer(scene);
	drawer->initWindows();
	glfwSetFramebufferSizeCallback(drawer->window, framebuffer_size_callback);
	// 添加灯光物体
	Primitive* lightCube = light->debugLightCube();
	scene->primitives.push_back(lightCube);
	lightCube->setup();
	// 添加几何体
	Primitive* cubeObj = new Primitive();
	cubeObj->shaderName = "basicTextureShader";
	cubeObj->textureUrls.emplace_back("container2.png");
	cubeObj->textureUrls.emplace_back("container2_specular.png");
	cubeObj->textureUrls.emplace_back("matrix.jpg");

	cubeObj->setup();
	scene->primitives.push_back(cubeObj);

	drawer->draw();

	system("pause");
	return 0;
}