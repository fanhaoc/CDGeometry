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
	};
	~Model() {};

	std::vector<Texture> textures_loaded;
	std::string shaderName = "basicTextureShader";
	Shader shader;
	// Ĭ��material
	glm::vec3 ambient = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
	glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f);
	float shininess = 32.0f;

	void Draw();
	void setupShader();
private:
	// ģ������, һ��model�������mesh��mesh����һ��������Ƭ
	std::vector<Mesh> meshes;
	std::string directory;
	// ����
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};