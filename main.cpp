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
#include "drawer/trick.h"
#include "drawer/geometry/model.h"
#include "drawer/primitives/superPrimitive.h"
#include "drawer/primitives/boxPrimitive.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main() {

	Assimp::Importer importer;

	Scene* scene = new Scene();

	Light* light = new Light(glm::vec3(1.2f, 1.0f, 2.0f), glm::vec3(1.0, 1.0, 1.0));
	scene->light = light;
		

	Drawer *drawer =new Drawer(scene);
	drawer->initWindows();
	glfwSetFramebufferSizeCallback(drawer->window, framebuffer_size_callback);

	// 添加灯光物体
	SuperPrimitive* lightCube = light->debugLightCube();
	//scene->primitives.push_back(lightCube);
	
	lightCube->setup();
	drawer->sps.push_back(lightCube);

	SuperPrimitive* cubeObj = new BoxPrimitive();
	cubeObj->shaderName = "phongColorShader";
	cubeObj->setup();
	drawer->sps.push_back(cubeObj);

	//加载模型

	//Model backbagModel("assets/backpack/backpack.obj");
	//backbagModel.setupShader();
	//scene->models.push_back(&backbagModel);
	std::cout << cubeObj->shader->ID << std::endl;

	drawer->draw();

	system("pause");
	return 0;
}