#include "light.h"

Light::Light(glm::vec3 pos, glm::vec3 color) {
	lightPos = pos;
	lightColor = color;
}

void Light::setup(unsigned int ID) {
	unsigned int lightPosLoc = glGetUniformLocation(ID, "lightPos");
	glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
	unsigned int lightColorLoc = glGetUniformLocation(ID, "lightColor");
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
}