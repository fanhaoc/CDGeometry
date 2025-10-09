#include "planePrimitive.h"

PlanePrimitive::PlanePrimitive() {}

PlanePrimitive::~PlanePrimitive() {}

void PlanePrimitive::setup() {
	std::cout << "plane" << std::endl;
	setupShader();
	setupVertexBuffer();
	setupUniforms();
	setupTextures();
}

void PlanePrimitive::setupShader() {
	std::string vsPath = Trick::solvePath("drawer/shaders/" + shaderName + ".vs");
	std::string fsPath = Trick::solvePath("drawer/shaders/" + shaderName + ".fs");
	std::string pre = "drawer/shaders/presetShader.fs";
	std::string preFsPath = Trick::solvePath(pre);
	shader = new Shader(vsPath.c_str(), fsPath.c_str(), preFsPath.c_str());
}

void PlanePrimitive::setupVertexBuffer() {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void PlanePrimitive::setupUniforms() {
	shader->use();
	glUniform3fv(glGetUniformLocation(shader->ID, "material.ambient"), 1, glm::value_ptr(ambient));
	glUniform3fv(glGetUniformLocation(shader->ID, "material.diffuse"), 1, glm::value_ptr(diffuse));
	glUniform3fv(glGetUniformLocation(shader->ID, "material.specular"), 1, glm::value_ptr(specular));
	glUniform1f(glGetUniformLocation(shader->ID, "material.shininess"), shininess);
	glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
}

void PlanePrimitive::setupTextures() {

}