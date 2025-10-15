#include "planePrimitive.h"

PlanePrimitive::PlanePrimitive() {}

PlanePrimitive::~PlanePrimitive() {}

void PlanePrimitive::setup() {
	std::cout << "plane" << std::endl;
	setupShader();
	setupVertexBuffer();
	//setupUniforms();
	setupTextures();
}

void PlanePrimitive::setupShader() {
	std::string vsPath = Trick::solvePath("drawer/shaders/" + shaderName + ".vs");
	std::string fsPath = Trick::solvePath("drawer/shaders/" + shaderName + ".fs");
	shader = new Shader(vsPath.c_str(), fsPath.c_str());

	std::string vsPath_01 = Trick::solvePath("drawer/shaders/" + shadowShaderName + ".vs");
	std::string fsPath_01 = Trick::solvePath("drawer/shaders/" + shadowShaderName + ".fs");
	std::string gsPath_01 = Trick::solvePath("drawer/shaders/" + shadowShaderName + ".gs");
	shadowShader = new Shader(vsPath_01.c_str(), fsPath_01.c_str(), gsPath_01.c_str());
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

void PlanePrimitive::setupUniforms(Shader* shaderq) {
	shaderq->use();
	glUniform3fv(glGetUniformLocation(shaderq->ID, "material.ambient"), 1, glm::value_ptr(ambient));
	glUniform3fv(glGetUniformLocation(shaderq->ID, "material.diffuse"), 1, glm::value_ptr(diffuse));
	glUniform3fv(glGetUniformLocation(shaderq->ID, "material.specular"), 1, glm::value_ptr(specular));
	glUniform1f(glGetUniformLocation(shaderq->ID, "material.shininess"), shininess);
	glUniformMatrix4fv(glGetUniformLocation(shaderq->ID, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
}

void PlanePrimitive::setupTextures() {

}