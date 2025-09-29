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
	Primitive* lightCube = light->debugLightCube();
	scene->primitives.push_back(lightCube);
	lightCube->setup();
	// 添加几何体
	Primitive* cubeObj = new Primitive();
	cubeObj->shaderName = "basicTextureShader";
	cubeObj->textureUrls.emplace_back("container2.png");
	//cubeObj->textureUrls.emplace_back("container2_specular.png");
	//cubeObj->textureUrls.emplace_back("matrix.jpg");
	cubeObj->isInstance = true;
	cubeObj->setup();
	scene->primitives.push_back(cubeObj);
	// 天空盒
	Primitive* skybox = new Primitive();
	skybox->shaderName = "skyboxShader";

	skybox->textureUrls.emplace_back("/skybox/right.jpg");
	skybox->textureUrls.emplace_back("/skybox/left.jpg");
	skybox->textureUrls.emplace_back("/skybox/top.jpg");
	skybox->textureUrls.emplace_back("/skybox/bottom.jpg");
	skybox->textureUrls.emplace_back("/skybox/front.jpg");
	skybox->textureUrls.emplace_back("/skybox/back.jpg");
	skybox->Vtype = POSITION;
	skybox->textureType = 1;
	skybox->modelMatrix = glm::scale(skybox->modelMatrix, glm::vec3(20, 20, 20));
	skybox->setup();
	scene->primitives.push_back(skybox);


	//加载模型

	//Model backbagModel("assets/backpack/backpack.obj");
	//backbagModel.setupShader();
	//scene->models.push_back(&backbagModel);
	std::cout << GL_MAX_VERTEX_UNIFORM_COMPONENTS << std::endl;

	drawer->draw();

	system("pause");
	return 0;
}