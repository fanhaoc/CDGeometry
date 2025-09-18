#pragma once
#include <string>
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
			// �ж��Ƿ�λ�ڵ�ǰ��ĿĿ¼
			if (token == projectName) {
				break;
			}
		}
		//std::string shaderPath = "drawer/shaders/";

		std::string fullPath = projectPath + path;
		return fullPath;
	};

	static bool almostEqual(const glm::vec3& a, const glm::vec3& b, float epsilon = 1e-6f) {
		return glm::all(glm::lessThan(glm::abs(a - b), glm::vec3(epsilon)));
	}
};
