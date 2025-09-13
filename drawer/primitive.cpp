#include "primitive.h"

Primitive::Primitive() {
	setupShader();
	setupBuffer();
};

int Primitive::setupBuffer() {

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	indicesSize = sizeof(indices) / sizeof(unsigned int);
	return 0;
}

int Primitive::setupShader() {
	//char currentPath[_MAX_PATH];
	std::string currentPath = std::filesystem::current_path().string();
	std::stringstream ss(currentPath);
	std::vector<std::string> tokens;
	std::string projectPath;
	std::string token;
	std::string projectName = "CDGeometry";
	while (std::getline(ss, token, '\\')) {
		projectPath += token;
		projectPath.push_back('/');
		// �ж��Ƿ�λ�ڵ�ǰ��ĿĿ¼
		if (token == projectName) {
			break;
		}
	}
	std::cout << projectPath << std::endl;
	std::string shaderPath = "drawer/shaders/";

	std::string fullPath = projectPath + shaderPath + shaderName;
	std::cout << fullPath << std::endl;
	std::string vs = fullPath + ".vs";
	std::string fs = fullPath + ".fs";
	shaderProgram = new Shader(vs.c_str(), fs.c_str());
	return 1;
}

void Primitive::update() {
	float timeValue = glfwGetTime();
	float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
	shaderProgram->use();
	int vertexColorLocation = glGetUniformLocation(shaderProgram->ID, "ourColor");
	glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
}