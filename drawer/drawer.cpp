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

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glEnable(GL_CULL_FACE);
		// 渲染指令
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		

		//装配view和projection矩阵
		glm::mat4 viewMatrix;
		glm::mat4 projMatrix;
		viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0, 0.0, -10.0));
		projMatrix = glm::perspective(glm::radians(45.0f), screenWidth / screenHeight, 0.1f, 1000.0f);

		// 绘制模型
		for (Model* model : scene->models) {
			model->shaders[0].use();
			// 传入view和projection矩阵，光照
			scene->light->setup(model->shaders[0].ID);
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);
			unsigned int viewLoc = glGetUniformLocation(model->shaders[0].ID, "view");
			unsigned int projLoc = glGetUniformLocation(model->shaders[0].ID, "projection");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera->viewMatrix));
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projMatrix));
			unsigned int viewPosLoc = glGetUniformLocation(model->shaders[0].ID, "viewPos");
			glUniform3fv(viewPosLoc, 1, glm::value_ptr(camera->cameraPos));

			model->modelMatrix = glm::mat4();

			model->Draw();

			model->shaders[1].use();
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);
			glDisable(GL_DEPTH_TEST);
			// 传入view和projection矩阵，光照
			scene->light->setup(model->shaders[1].ID);
			glUniformMatrix4fv(glGetUniformLocation(model->shaders[1].ID, "view"), 1, GL_FALSE, glm::value_ptr(camera->viewMatrix));
			glUniformMatrix4fv(glGetUniformLocation(model->shaders[1].ID, "projection"), 1, GL_FALSE, glm::value_ptr(projMatrix));
			glUniform3fv(glGetUniformLocation(model->shaders[1].ID, "viewPos"), 1, glm::value_ptr(camera->cameraPos));

			model->modelMatrix = glm::scale(model->modelMatrix, glm::vec3(1.1, 1.1, 1.1));

			model->Draw(1);
			glStencilMask(0xFF);
			glEnable(GL_DEPTH_TEST);
		}

		
		//// 绘制几何体
		//for (Primitive* pri : scene->primitives) {
		//	pri->shaderProgram->use();

		//	unsigned int texture_index = -1;
		//	for (unsigned int texture : pri->textures) {
		//		
		//		glActiveTexture(GL_TEXTURE0 + ++texture_index);
		//		glBindTexture(GL_TEXTURE_2D, texture);
		//	}
		//	glBindVertexArray(pri->VAO);

		//	

		//	// 传入view和projection矩阵，光照
		//	scene->light->setup(pri->shaderProgram->ID);
		//	unsigned int viewLoc = glGetUniformLocation(pri->shaderProgram->ID, "view");
		//	unsigned int projLoc = glGetUniformLocation(pri->shaderProgram->ID, "projection");
		//	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera->viewMatrix));
		//	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projMatrix));
		//	unsigned int viewPosLoc = glGetUniformLocation(pri->shaderProgram->ID, "viewPos");
		//	glUniform3fv(viewPosLoc, 1, glm::value_ptr(camera->cameraPos));
		//	
		//	pri->update();
		//	//glDrawElements(GL_TRIANGLES, pri->indicesSize, GL_UNSIGNED_INT, 0);
		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		//}
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
	float cameraSpeed = deltaFrame;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->moveCameraPos(camera->getForwardDir(yaw), cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->moveCameraPos(-camera->getForwardDir(yaw), cameraSpeed);
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