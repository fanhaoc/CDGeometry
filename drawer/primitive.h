#pragma once
#include <filesystem>
#include <vector>
//#include <strstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "shaders/shader.h"
#include "trick.h"

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

    float texCoords[6] = {
        0.0f, 0.0f, // ���½�
        1.0f, 0.0f, // ���½�
        0.5f, 1.0f  // ����
    };

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
    std::string shaderName = "defaultShader";
    
    Shader* shaderProgram;
    int indicesSize = 0;

	int setupBuffer();
    int setupShader();
    int setupTexture();
    void update();

private:

};