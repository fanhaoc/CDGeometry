#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Eigen/Dense>

#include "drawer/drawer.h"
#include "drawer/primitive.h"
#include "geometryProcess/cubicSpline.h"
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
int main() {
		std::cout << "Hello, World!" << std::endl;
		

		CubicSpline *cspline = new CubicSpline();
		cspline->addPoint(-1.87, -19.21339);
		cspline->addPoint(1.68, 21.03558);
		cspline->addPoint(12.04, 28.70792);
		cspline->addPoint(31.3, -32.11539);
		cspline->solver();
		std::cout << "spline param: a:" << cspline->spline.a[0] 
			<<"b:" << cspline->spline.b[0]
			<<"c:" << cspline->spline.c[0]
			<<"d:" << cspline->spline.d[0]
			<< std::endl;

		Drawer drawer;
		drawer.initWindows();
		glfwSetFramebufferSizeCallback(drawer.window, framebuffer_size_callback);
		// 添加几何体
		drawer.primitives.push_back(new Primitive());

		drawer.draw();

		system("pause");
		return 0;
}