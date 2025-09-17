#include "drawer.h"

int Drawer::initWindows(){
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glViewport(0, 0, screenWidth, screenHeight);
	return 0;
}
int Drawer::draw(){
	while (!glfwWindowShouldClose(window))
	{
		// 输入
		processInput(window);

		// 渲染指令
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);

		//装配view和projection矩阵
		glm::mat4 viewMatrix;
		glm::mat4 projMatrix;
		viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0, 0.0, -3.0));
		projMatrix = glm::perspective(glm::radians(45.0f), screenWidth / screenHeight, 0.1f, 100.0f);

		// 绘制几何体
		for (Primitive* pri : primitives) {
			pri->shaderProgram->use();
			glBindTexture(GL_TEXTURE_2D, pri->texture);
			glBindVertexArray(pri->VAO);

			// 传入view和projection矩阵
			unsigned int viewLoc = glGetUniformLocation(pri->shaderProgram->ID, "view");
			unsigned int projLoc = glGetUniformLocation(pri->shaderProgram->ID, "projection");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera->viewMatrix));
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projMatrix));
			
			pri->update();
			//glDrawElements(GL_TRIANGLES, pri->indicesSize, GL_UNSIGNED_INT, 0);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		//// 检查并调用事件，交换缓冲
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}
void Drawer::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}