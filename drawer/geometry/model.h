#pragma once
#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../shaders/shader.h"
#include "mesh.h"
#include "../trick.h"

class Model
{
public:
	Model(char* path) {
		loadModel(path);
		setupShader();
	};
	~Model() {};

	std::vector<Texture> textures_loaded;
	std::string shaderNames[2] = { "basicTextureShader" , "outlineColorShader"};
	Shader shaders[2];
	//std::string shaderName = "basicTextureShader";
	//Shader shader;
	// 默认material
	glm::vec3 ambient = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
	glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f);
	float shininess = 32.0f;
	glm::mat4 modelMatrix;

	void Draw(unsigned int index = 0);
	void setupShader();
	void changeShader(std::string shaderName);
private:
	// 模型数据, 一个model包含多个mesh，mesh不是一个三角面片
	std::vector<Mesh> meshes;
	std::string directory;
	// 函数
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};