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

void Drawer::getFrameBuffer(unsigned int &framebuffer, unsigned int &fbTexture) {

	glGenFramebuffers(1, &framebuffer);
	
	// 生成纹理附加到帧缓冲上
	glGenTextures(1, &fbTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, fbTexture);
	for(unsigned int i=0;i!=6;++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	//// 设置边界颜色，让超出光视锥的部分不被阴影覆盖
	//float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// 将纹理附加到帧缓冲对象
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, fbTexture, 0);
	// 不渲染任何颜色
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	//// 渲染缓冲对象
	//unsigned int rbo;
	//glGenRenderbuffers(1, &rbo);
	//glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	//
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	//glBindRenderbuffer(GL_RENDERBUFFER, 0);
	//
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	//	std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	// 重要：检查帧缓冲完整性
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Shadow Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

int Drawer::draw(){
	glfwSetCursorPosCallback(window, mouse_callback);
	unsigned int framebuffer, shadowCubeMap;
	getFrameBuffer(framebuffer, shadowCubeMap);
	// 创建ubo
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO, 0, 3 * sizeof(glm::mat4));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	// 计算lightspacematrix
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	//lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 10.0f);
	lightProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 20.0f);
	shadowTransforms.push_back(lightProjection * glm::lookAt(scene->light->lightPos, scene->light->lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(lightProjection * glm::lookAt(scene->light->lightPos, scene->light->lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(lightProjection * glm::lookAt(scene->light->lightPos, scene->light->lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	shadowTransforms.push_back(lightProjection * glm::lookAt(scene->light->lightPos, scene->light->lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	shadowTransforms.push_back(lightProjection * glm::lookAt(scene->light->lightPos, scene->light->lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(lightProjection * glm::lookAt(scene->light->lightPos, scene->light->lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
	//lightView = glm::lookAt(scene->light->lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0, 1.0, 0.0));
	//lightSpaceMatrix = lightProjection * lightView;
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(lightSpaceMatrix));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

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

		//装配view和projection矩阵
		glm::mat4 viewMatrix;
		glm::mat4 projMatrix;
		viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0, 1.0, 80.0));
		projMatrix = glm::perspective(glm::radians(45.0f), screenWidth / screenHeight, 0.1f, 1000.0f);

		// 绑定ubo，并更新数据
		glBindBuffer(GL_UNIFORM_BUFFER, UBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projMatrix));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera->viewMatrix));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		drawPrimitives(framebuffer, shadowCubeMap);
		glViewport(0, 0, screenWidth, screenHeight);
		drawPrimitives(0, shadowCubeMap);


		//// 检查并调用事件，交换缓冲
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}

void Drawer::drawPrimitives(const unsigned int &fb, const unsigned int &shadowMap) {
	// 状态切换
	glBindFramebuffer(GL_FRAMEBUFFER, fb);
	glEnable(GL_DEPTH_TEST);
	//if (fb == 0) {
	//	glCullFace(GL_BACK);
	//}
	//else {
	//	glCullFace(GL_FRONT);
	//}
	//glEnable(GL_STENCIL_TEST);
	//glEnable(GL_CULL_FACE);
	// 渲染指令
	glClearColor(0.5f, 0.4f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (SuperPrimitive* sp : scene->primitives) {
		Shader* shaderPtr = nullptr;
		if (fb == 0) {
			// 普通绘制
			sp->shader->use();
			shaderPtr = sp->shader;
			// 传入shadowMap
			glActiveTexture(GL_TEXTURE10);
			glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap);
			glUniform1i(glGetUniformLocation(shaderPtr->ID, "shadowMap"), 10);
			// 传入texture
			for (unsigned int i = 0; i < sp->textures.size(); ++i) {
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, sp->textures[i]);
				glUniform1i(glGetUniformLocation(shaderPtr->ID, sp->textureNames[i].c_str()), i);
			}
		}
		else {
			// 阴影贴图绘制
			sp->shadowShader->use();
			shaderPtr = sp->shadowShader;
			// 将shadowTransforms传入shader
			for(int i=0;i!=6;++i)
				glUniformMatrix4fv(glGetUniformLocation(shaderPtr->ID, ("shadowMatrices[" + std::to_string(i) + "]").c_str()), 1, GL_FALSE, glm::value_ptr(shadowTransforms[i]));
		}
		glBindVertexArray(sp->VAO);

		sp->setupUniforms(shaderPtr);

		// 设置matrices uniform块绑定到0
		unsigned int uniformBlockMatrices = glGetUniformBlockIndex(shaderPtr->ID, "Matrices");
		glUniformBlockBinding(shaderPtr->ID, uniformBlockMatrices, 0);
		glUniform3fv(glGetUniformLocation(shaderPtr->ID, "viewPos"), 1, glm::value_ptr(camera->cameraPos));
		glUniform1f(glGetUniformLocation(shaderPtr->ID, "far_plane"), 20.0);
		scene->light->setup(shaderPtr->ID);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

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