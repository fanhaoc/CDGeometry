#pragma once
#include <string>
#include <filesystem>
#include <sstream>  
//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
class Trick
{
public:
	Trick();
	~Trick();

	static std::string solvePath(std::string& path) {
		std::string currentPath = std::filesystem::current_path().string();
		std::stringstream ss(currentPath);
		std::vector<std::string> tokens;
		std::string projectPath;
		std::string token;
		std::string projectName = "CDGeometry";
		while (std::getline(ss, token, '\\')) {
			projectPath += token;
			projectPath.push_back('/');
			// 判断是否位于当前项目目录
			if (token == projectName) {
				break;
			}
		}
		//std::string shaderPath = "drawer/shaders/";

		std::string fullPath = projectPath + path;
		return fullPath;
	};

	static unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false) {
		std::string fileName = std::string(path);
		fileName = directory + '/' + fileName;

		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nrComponents, 0);
		if (data) {
			GLenum format = GL_RGB;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}else{
			std::cout << "Texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
		}

		return textureID;
	}

	static bool almostEqual(const glm::vec3& a, const glm::vec3& b, float epsilon = 1e-6f) {
		return glm::all(glm::lessThan(glm::abs(a - b), glm::vec3(epsilon)));
	}
};
