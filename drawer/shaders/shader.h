#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	// ����ID
	unsigned int ID;
	Shader() = default;
	Shader(const char* vertexPath, const char* fragmentPath, const char* preFragmentPath);
	~Shader() {};
	// ʹ�ó���
	void use();
	// uniform���ߺ�������ʲô�ã���
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

private:

};
