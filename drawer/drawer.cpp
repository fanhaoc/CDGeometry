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
	glfwSetWindowUserPointer(window, this);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glViewport(0, 0, screenWidth, screenHeight);
	return 0;
}
int Drawer::draw(){
	glfwSetCursorPosCallback(window, mouse_callback);
	while (!glfwWindowShouldClose(window))
	{
		// 计算帧时间
		float currentFrame = glfwGetTime();
		deltaFrame = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// 输入
		processInput(window);
		// 判断是否需要更新相机
		if (!Trick::almostEqual(cameraFront, camera->cameraFront)){
			camera->cameraFront = cameraFront;
			camera->updateCameraParams();
		}

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
	float cameraSpeed = 2.5f * deltaFrame;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->moveCameraPos(-camera->cameraDirection, cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->moveCameraPos(camera->cameraDirection, cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->moveCameraPos(-camera->cameraRight, cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->moveCameraPos(camera->cameraRight, cameraSpeed);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		mouseLeftEnter = true;
		
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		mouseLeftEnter = false;
		lastXpos = -10000.0;
	}
		
}

void Drawer::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	Drawer* drawer = static_cast<Drawer*>(glfwGetWindowUserPointer(window));
	if (drawer->mouseLeftEnter) {
		if (drawer->lastXpos == -10000.0) {
			// 初始化坐标
			drawer->lastXpos = xpos;
			drawer->lastYpos = ypos;
		}
		float xOffset = xpos - drawer->lastXpos;
		float yOffset = ypos - drawer->lastYpos;
		drawer->lastXpos = xpos;
		drawer->lastYpos = ypos;
		float sensitivity = 0.05f;
		xOffset *= sensitivity;
		yOffset *= sensitivity;
		drawer->yaw += xOffset;
		drawer->pitch -= yOffset;
		if (drawer->pitch > 89.0f)
			drawer->pitch = 89.0f;
		if (drawer->pitch < -89.0f)
			drawer->pitch = -89.0f;
		glm::vec3 front;
		front.x = cos(glm::radians(drawer->pitch)) * cos(glm::radians(drawer->yaw));
		front.y = sin(glm::radians(drawer->pitch));
		front.z = cos(glm::radians(drawer->pitch)) * sin(glm::radians(drawer->yaw));
		drawer->cameraFront = glm::normalize(front);
	}

}