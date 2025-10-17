#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../shaders/shader.h"
#include "../trick.h"

class SuperPrimitive {
public:
	SuperPrimitive() {};
	~SuperPrimitive() {};

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	Shader* shader;
	Shader* shadowShader;

	std::string shaderName = "basicColorShader";
	std::string shadowShaderName = "simpleDepthShader";

	std::vector<std::string> textureUrls;
	std::vector<unsigned int> textures;

	glm::mat4 modelMatrix;
	// 默认颜色属性
	glm::vec3 ambient = glm::vec3(0.5, 0.5, 0.5);
	glm::vec3 diffuse = glm::vec3(0.8, 0.5, 0.5);
	glm::vec3 specular = glm::vec3(0.5, 0.5, 0.5);
	float shininess = 32.0f;

	// pbr属性
	glm::vec3 albedo = glm::vec3(0.5, 0.0, 0.0);
	float metallic = 1.0f;
	float roughness = 0.3f;
	float ao = 1.0f;
	std::string textureNames[3] = { "albedoMap", "metallicMap", "roughnessMap" };

	virtual void setup() {
		std::cout << "super" << std::endl;
	};
	virtual void setupUniforms(Shader* shader) {};
	void update();
	void draw();

};