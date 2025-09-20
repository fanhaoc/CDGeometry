#include "light.h"

Light::Light(glm::vec3 pos, glm::vec3 color) {
	lightPos = pos;
	lightColor = color;
}

void Light::setup(unsigned int ID) {
	unsigned int lightPosLoc = glGetUniformLocation(ID, "light.position");
	glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
	unsigned int ambientLoc = glGetUniformLocation(ID, "light.ambient");
	glUniform3fv(ambientLoc, 1, glm::value_ptr(ambient));
	unsigned int diffuseLoc = glGetUniformLocation(ID, "light.diffuse");
	glUniform3fv(diffuseLoc, 1, glm::value_ptr(diffuse));
	unsigned int specularLoc = glGetUniformLocation(ID, "light.specular");
	glUniform3fv(specularLoc, 1, glm::value_ptr(specular));
}

Primitive* Light::debugLightCube() {
	Primitive* lightCube = new Primitive();
	lightCube->shaderName = "basicColorShader";
	lightCube->modelMatrix = glm::translate(lightCube->modelMatrix, lightPos);
	lightCube->modelMatrix = glm::scale(lightCube->modelMatrix, glm::vec3(0.2f));
	lightCube->diffuse = diffuse;
	return lightCube;
}