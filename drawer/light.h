#pragma once 
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Light
{
public:
	Light(glm::vec3 pos, glm::vec3 color);
	~Light();

	glm::vec3 lightPos;
	glm::vec3 lightColor;

	void setup(unsigned int ID);
private:

};