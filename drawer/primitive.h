#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "program.h"
#include "shaders/shader.h"

class Primitive
{
public:
	Primitive();
	~Primitive() {}
	;
    float vertices[12] = {
        0.5f, 0.5f, 0.0f,   // 右上角
        0.5f, -0.5f, 0.0f,  // 右下角
        -0.5f, -0.5f, 0.0f, // 左下角
        -0.5f, 0.5f, 0.0f   // 左上角
    };

    unsigned int indices[6] = {
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	Shader* shaderProgram = new Shader("G:/0_cfh/projects/learnOpenGL/drawer/shaders/defaultShader.vs", "G:/0_cfh/projects/learnOpenGL/drawer/shaders/defaultShader.fs");
    int indicesSize = 0;

	int setupBuffer();
    void update();

private:

};