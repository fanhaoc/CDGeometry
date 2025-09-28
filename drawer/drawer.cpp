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

unsigned int Drawer::getFrameBuffer() {
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// ���������ӵ�֡������
	glGenTextures(1, &fbTexture);
	glBindTexture(GL_TEXTURE_2D, fbTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB,GL_UNSIGNED_BYTE,NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	// �������ӵ�֡�������
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbTexture, 0);

	// ��Ⱦ�������
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return framebuffer;
}

int Drawer::draw(){
	glfwSetCursorPosCallback(window, mouse_callback);
	getFrameBuffer();
	while (!glfwWindowShouldClose(window))
	{
		// ����֡ʱ��
		float currentFrame = glfwGetTime();
		deltaFrame = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// ����
		processInput(window);
		// �ж��Ƿ���Ҫ�������
		if (!Trick::almostEqual(cameraFront, camera->cameraFront)){
			camera->cameraFront = cameraFront;
			camera->updateCameraParams();
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		//glEnable(GL_CULL_FACE);
		// ��Ⱦָ��
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		

		//װ��view��projection����
		glm::mat4 viewMatrix;
		glm::mat4 projMatrix;
		viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0, 0.0, -10.0));
		projMatrix = glm::perspective(glm::radians(45.0f), screenWidth / screenHeight, 0.1f, 1000.0f);

		//// ����ģ��
		//for (Model* model : scene->models) {
		//	model->shaders[0].use();
		//	// ����view��projection���󣬹���
		//	scene->light->setup(model->shaders[0].ID);
		//	glStencilFunc(GL_ALWAYS, 1, 0xFF);
		//	glStencilMask(0xFF);
		//	unsigned int viewLoc = glGetUniformLocation(model->shaders[0].ID, "view");
		//	unsigned int projLoc = glGetUniformLocation(model->shaders[0].ID, "projection");
		//	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera->viewMatrix));
		//	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projMatrix));
		//	unsigned int viewPosLoc = glGetUniformLocation(model->shaders[0].ID, "viewPos");
		//	glUniform3fv(viewPosLoc, 1, glm::value_ptr(camera->cameraPos));

		//	model->modelMatrix = glm::mat4();

		//	model->Draw();

		//	model->shaders[1].use();
		//	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		//	glStencilMask(0x00);
		//	glDisable(GL_DEPTH_TEST);
		//	// ����view��projection���󣬹���
		//	scene->light->setup(model->shaders[1].ID);
		//	glUniformMatrix4fv(glGetUniformLocation(model->shaders[1].ID, "view"), 1, GL_FALSE, glm::value_ptr(camera->viewMatrix));
		//	glUniformMatrix4fv(glGetUniformLocation(model->shaders[1].ID, "projection"), 1, GL_FALSE, glm::value_ptr(projMatrix));
		//	glUniform3fv(glGetUniformLocation(model->shaders[1].ID, "viewPos"), 1, glm::value_ptr(camera->cameraPos));

		//	model->modelMatrix = glm::scale(model->modelMatrix, glm::vec3(1.1, 1.1, 1.1));

		//	model->Draw(1);
		//	glStencilMask(0xFF);
		//	glEnable(GL_DEPTH_TEST);
		//}

		
		// ���Ƽ�����
		
		for (Primitive* pri : scene->primitives) {
			pri->shaderProgram->use();

			unsigned int texture_index = -1;
			for (unsigned int texture : pri->textures) {
				
				glActiveTexture(GL_TEXTURE0 + ++texture_index);
				glBindTexture(GL_TEXTURE_2D, texture);
			}
			glBindVertexArray(pri->VAO);
			// ����view��projection���󣬹���
			scene->light->setup(pri->shaderProgram->ID);
			unsigned int viewLoc = glGetUniformLocation(pri->shaderProgram->ID, "view");
			unsigned int projLoc = glGetUniformLocation(pri->shaderProgram->ID, "projection");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera->viewMatrix));
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projMatrix));
			unsigned int viewPosLoc = glGetUniformLocation(pri->shaderProgram->ID, "viewPos");
			glUniform3fv(viewPosLoc, 1, glm::value_ptr(camera->cameraPos));
			
			pri->update();
			//glDrawElements(GL_TRIANGLES, pri->indicesSize, GL_UNSIGNED_INT, 0);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		

		//// ��鲢�����¼�����������
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
			// ��ʼ������
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