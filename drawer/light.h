#pragma once 
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "primitive.h"

class Light
{
public:
	Light(glm::vec3 pos, glm::vec3 color);
	~Light();

	glm::vec3 lightPos;
	glm::vec3 lightColor;
	glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);

	void setup(unsigned int ID);
	Primitive* debugLightCube();
private:

};