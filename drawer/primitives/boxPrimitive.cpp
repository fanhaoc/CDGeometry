#include "boxPrimitive.h"

BoxPrimitive::BoxPrimitive(){}

BoxPrimitive::~BoxPrimitive(){}

void BoxPrimitive::setup(){
	std::cout << "box" << std::endl;
	setupShader();
	setupVertexBuffer();
	//setupUniforms();
	setupTextures();
}

void BoxPrimitive::setupShader() {
	std::string vsPath = Trick::solvePath("drawer/shaders/" + shaderName + ".vs");
	std::string fsPath = Trick::solvePath("drawer/shaders/" + shaderName + ".fs");
	shader = new Shader(vsPath.c_str(), fsPath.c_str());

	std::string vsPath_01 = Trick::solvePath("drawer/shaders/" + shadowShaderName + ".vs");
	std::string fsPath_01 = Trick::solvePath("drawer/shaders/" + shadowShaderName + ".fs");
	std::string gsPath_01 = Trick::solvePath("drawer/shaders/" + shadowShaderName + ".gs");
	shadowShader = new Shader(vsPath_01.c_str(), fsPath_01.c_str(), gsPath_01.c_str());
}

void BoxPrimitive::setupVertexBuffer() {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BoxPrimitive::setupUniforms(Shader *shader) {
	shader->use();
	glUniform3fv(glGetUniformLocation(shader->ID, "material.ambient"), 1, glm::value_ptr(ambient));
	glUniform3fv(glGetUniformLocation(shader->ID, "material.diffuse"), 1, glm::value_ptr(diffuse));
	glUniform3fv(glGetUniformLocation(shader->ID, "material.specular"), 1, glm::value_ptr(specular));
	glUniform1f(glGetUniformLocation(shader->ID, "material.shininess"), shininess);
	glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// pbrÊôÐÔ
	glUniform3fv(glGetUniformLocation(shader->ID, "albedo"), 1, glm::value_ptr(albedo));
	glUniform1f(glGetUniformLocation(shader->ID, "metallic"), metallic);
	glUniform1f(glGetUniformLocation(shader->ID, "roughness"), roughness);
	glUniform1f(glGetUniformLocation(shader->ID, "ao"), ao);
}

void BoxPrimitive::setupTextures() {
	std::string a1 = "assets/rustediron";
	std::string Path01 = Trick::solvePath(a1);
	textures.push_back(Trick::TextureFromFile("rustediron2_basecolor.png", Path01.c_str()));
	textures.push_back(Trick::TextureFromFile("rustediron2_metallic.png", Path01.c_str()));
	textures.push_back(Trick::TextureFromFile("rustediron2_roughness.png", Path01.c_str()));
}