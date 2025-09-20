#include "shader.h"

Shader::Shader(const char* veretxPath, const char* fragmentPath, const char* preFragmentPath) {
	// ��ȡ�ļ���ȡsource
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream pfShaderFile;
	// ��֤ifstream��������׳��쳣
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	pfShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		// ���ļ�
		vShaderFile.open(veretxPath);
		fShaderFile.open(fragmentPath);
		pfShaderFile.open(preFragmentPath);
		std::stringstream vShaderStream, fShaderStream, pfShaderStream;
		// ��ȡ�ļ��Ļ������ݵ���������
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		pfShaderStream << pfShaderFile.rdbuf();
		//  �ر��ļ�
		vShaderFile.close();
		fShaderFile.close();
		pfShaderFile.close();
		// ת����������string
		vertexCode = vShaderStream.str();
		fragmentCode = pfShaderStream.str() + fShaderStream.str();
	}
	catch(std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// ������ɫ��
	unsigned int vertex, fragment;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(vertex);
	glCompileShader(fragment);
	// ��ӡ�����������еĻ���
	int success;
	char infoLog[512];
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// ������ɫ��
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	// ��ӡ���Ӵ�������еĻ���
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	// ɾ����ɫ��
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use() {
	glUseProgram(ID);
}
