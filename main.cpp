#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Eigen/Dense>
#include <assimp/Importer.hpp>
#include <Windows.h>


#include "drawer/drawer.h"
#include "geometryProcess/cubicSpline.h"
#include "drawer/scene.h"
#include "drawer/light.h"
#include "drawer/trick.h"
#include "drawer/geometry/model.h"
#include "drawer/primitives/superPrimitive.h"
#include "drawer/primitives/boxPrimitive.h"
#include "drawer/primitives/planePrimitive.h"



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main() {

	Assimp::Importer importer;

	Scene* scene = new Scene();

	Light* light = new Light(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0, 1.0, 1.0));
	scene->light = light;
		

	Drawer *drawer =new Drawer(scene);
	drawer->initWindows();
	glfwSetFramebufferSizeCallback(drawer->window, framebuffer_size_callback);

	//// ��ӵƹ�����
	//SuperPrimitive* lightCube = light->debugLightCube();
	////scene->primitives.push_back(lightCube);
	//
	//lightCube->setup();
	//scene->primitives.push_back(lightCube);

	SuperPrimitive* cubeObj = new BoxPrimitive();
	cubeObj->shaderName = "pbrShader";
	cubeObj->modelMatrix = glm::translate(cubeObj->modelMatrix, glm::vec3(0.0, 1.0, -1.0));
	cubeObj->setup();
	scene->primitives.push_back(cubeObj);


	SuperPrimitive* cubeObj02 = new BoxPrimitive();
	cubeObj02->shaderName = "phongColorShader";
	// ��z����ƽ��
	cubeObj02->modelMatrix = glm::translate(cubeObj02->modelMatrix, glm::vec3(-1.0, 0.5, 0.0));
	cubeObj02->setup();
	scene->primitives.push_back(cubeObj02);

	// ����
	SuperPrimitive* planeObj = new PlanePrimitive();
	planeObj->shaderName = "phongColorShader";
	planeObj->diffuse = glm::vec3(1.0, 1.0, 1.0); 
	planeObj->ambient = glm::vec3(1.0, 1.0, 1.0);
	planeObj->specular = glm::vec3(1.0, 1.0, 1.0);
	planeObj->modelMatrix = glm::scale(planeObj->modelMatrix, glm::vec3(10.0, 1.0, 10.0));
	planeObj->setup();
	scene->primitives.push_back(planeObj);

	//����ģ��

	//Model backbagModel("assets/backpack/backpack.obj");
	//backbagModel.setupShader();
	//scene->models.push_back(&backbagModel);
	std::cout << cubeObj->shader->ID << std::endl;

	drawer->draw();

	system("pause");
	return 0;
}