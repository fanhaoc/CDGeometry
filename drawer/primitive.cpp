#include "primitive.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Primitive::Primitive() {

};

int Primitive::setup() {
	setupShader();
	setupBuffer();
	setupUniform();
	//setupTexture();
	return 1;
}

int Primitive::setupBuffer() {

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	//glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	indicesSize = sizeof(indices) / sizeof(unsigned int);
	return 0;
}

int Primitive::setupShader() {
	std::string vsPath = Trick::solvePath("drawer/shaders/" + shaderName + ".vs");
	std::string fsPath = Trick::solvePath("drawer/shaders/" + shaderName + ".fs");
	std::string pre = "drawer/shaders/presetShader.fs";
	std::string preFsPath = Trick::solvePath(pre);
	shaderProgram = new Shader(vsPath.c_str(), fsPath.c_str(), preFsPath.c_str());
	return 1;
}

int Primitive::setupTexture() {
	int width, height, nrChannel;
	std::string image = "assets/container.jpg";
	std::string imagePath = Trick::solvePath(image);
	std::cout << imagePath << std::endl;
	unsigned char* data = stbi_load(imagePath.c_str(), &width, &height, &nrChannel, 0);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "fail to load texture" << std::endl;
	}
	stbi_image_free(data);
	return 1;
}

int Primitive::setupUniform() {
	shaderProgram->use();
	// 基础颜色和模型矩阵
	unsigned int modelLoc = glGetUniformLocation(shaderProgram->ID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	unsigned int ambientLoc = glGetUniformLocation(shaderProgram->ID, "material.ambient");
	glUniform3fv(ambientLoc, 1,glm::value_ptr(ambient));
	unsigned int diffuseLoc = glGetUniformLocation(shaderProgram->ID, "material.diffuse");
	glUniform3fv(diffuseLoc, 1, glm::value_ptr(diffuse));
	unsigned int specularLoc = glGetUniformLocation(shaderProgram->ID, "material.specular");
	glUniform3fv(specularLoc, 1, glm::value_ptr(specular));
	unsigned int shininessLoc = glGetUniformLocation(shaderProgram->ID, "material.shininess");
	glUniform1f(shininessLoc,shininess);
	return 1;
}

void Primitive::update() {
	//float timeValue = glfwGetTime();
	//float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
	//shaderProgram->use();
	//int vertexColorLocation = glGetUniformLocation(shaderProgram->ID, "ourColor");
	//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
	//glm::mat4 modelMatrix;
	//modelMatrix = glm::rotate(modelMatrix, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));


}