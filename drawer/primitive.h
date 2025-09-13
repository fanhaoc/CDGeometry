#pragma once
#include <filesystem>
#include <vector>
//#include <strstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaders/shader.h"

class Primitive
{
public:
	Primitive();
	~Primitive() {}
	;
    float vertices[12] = {
        0.5f, 0.5f, 0.0f,   // ���Ͻ�
        0.5f, -0.5f, 0.0f,  // ���½�
        -0.5f, -0.5f, 0.0f, // ���½�
        -0.5f, 0.5f, 0.0f   // ���Ͻ�
    };

    unsigned int indices[6] = {
        0, 1, 3, // ��һ��������
        //1, 2, 3  // �ڶ���������
    };

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
    std::string shaderName = "defaultShader";
    
    Shader* shaderProgram;
    int indicesSize = 0;

	int setupBuffer();
    int setupShader();
    void update();

private:

};