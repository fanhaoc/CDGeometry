#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	// 程序ID
	unsigned int ID;
	Shader(const char* vertexPath, const char* fragmentPath, const char* preFragmentPath);
	~Shader();
	// 使用程序
	void use();
	// uniform工具函数（有什么用？）
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

private:

};
