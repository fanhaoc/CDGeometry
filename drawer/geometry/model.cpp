#include "model.h"

void Model::Draw() {
	unsigned int ambientLoc = glGetUniformLocation(shader.ID, "material.ambient");
	glUniform3fv(ambientLoc, 1, glm::value_ptr(ambient));
	unsigned int diffuseLoc = glGetUniformLocation(shader.ID, "material.diffuse");
	glUniform3fv(diffuseLoc, 1, glm::value_ptr(diffuse));
	unsigned int specularLoc = glGetUniformLocation(shader.ID, "material.specular");
	glUniform3fv(specularLoc, 1, glm::value_ptr(specular));
	unsigned int shininessLoc = glGetUniformLocation(shader.ID, "material.shininess");
	glUniform1f(shininessLoc, shininess);

	for (auto mesh : meshes) {
		mesh.Draw(shader);
	}
}
void Model::setupShader() {
	std::string vsPath = Trick::solvePath("drawer/shaders/" + shaderName + ".vs");
	std::string fsPath = Trick::solvePath("drawer/shaders/" + shaderName + ".fs");
	std::string pre = "drawer/shaders/presetShader.fs";
	std::string preFsPath = Trick::solvePath(pre);
	shader =  Shader(vsPath.c_str(), fsPath.c_str(), preFsPath.c_str());
}

void Model::loadModel(std::string path) {
	Assimp::Importer importer;
	std::string solvePath = Trick::solvePath(path);
	const aiScene *scene = importer.ReadFile(solvePath, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	directory = solvePath.substr(0, solvePath.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
	// ����ڵ����������
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// ���ӽڵ��ظ�
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene *scene) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
		Vertex vertex;
		// ������λ�á����ߺ�����
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		if (mesh->mTextureCoords[0]) { // �����Ƿ�����������
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.Texcoords = vec;
		}
		else {
			vertex.Texcoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}
	// ��������
	for (unsigned int i = 0; i < mesh->mNumFaces;i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// �������
	if (mesh->mMaterialIndex > 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}
	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i) {
		aiString str;
		mat->GetTexture(type, i, &str);
		// �ж�texture�Ƿ��Ѿ�����
		bool skip = false;
		for (unsigned int j = 0; j != textures_loaded.size(); ++j) {
			if (std::strcmp(textures_loaded[j].path.data, str.C_Str()) == 0) {
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip) {
			Texture texture;
			texture.id = Trick::TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture); // ��ӵ��Ѽ��ص�������
		}
	}
	return textures;
}