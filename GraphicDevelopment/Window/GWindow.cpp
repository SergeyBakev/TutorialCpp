#include "stdafx.h"
#include "GWindow.h"
#include "Window2dManager.h"
GWindow2d::GWindow2d(size_t width, size_t height, std::string_view title):
	width_(width), height_(height),title_(title)
{
	
	if (!glfwInit())
		throw std::runtime_error("Could not initialize GLFW");

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	
	window_ = glfwCreateWindow(static_cast<int>(width_), static_cast<int>(height_), title_.c_str(), NULL, NULL);
	if (!window_)
	{
		glfwTerminate();
		throw std::runtime_error("Could not initialize GLFW");
	}
	glfwMakeContextCurrent(window_);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		throw std::runtime_error("Could not initialize GLEW");

	Manadger()->Register(this, window_);
	//glViewport(0, 0, width_, height_);
}

bool GWindow2d::IsShouldClose() const
{
	return glfwWindowShouldClose(window_);
}

void GWindow2d::SwapBuffer() const
{
	glfwSwapBuffers(window_);
	
}

GLFWwindow* GWindow2d::Handle() const
{
	return window_;
}

void GWindow2d::Scale(double xoff, double yoff)
{
	scale_x_ = xoff;
	scale_y_ = yoff;
	//glPushMatrix();
	//glLoadIdentity();
	glScaled(scale_x_, scale_y_,1.0);
	//glPopMatrix();
}

void GWindow2d::RegisterWindow() const
{
}

GWindow2dManger* GWindow2d::Manadger()
{
	return GWindow2dManger::Instanse();
}
