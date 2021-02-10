#include "stdafx.h"
#include "GWindow.h"
#include "..\GL\GVertexBufferObject.h"
#include "..\GL\GPoint.h"
#include "..\Resources\ResourceManager.h"
#include "Window2dManager.h"

using namespace Common::Graphic;
using namespace Common::Resources;

void print(glm::mat4& m);
void print(glm::vec3& m);
void print(glm::vec4& m);

bool isMouseBnt1Presed = false;
float fovy = 45;
float scale = 1.1f;

#pragma region Window callback

void scroled(GLFWwindow* win, double xoffset, double yoffset)
{
	GWindow2d* window = GWindow2dManger::Instanse()->GetWindow(win);
	auto mat = glm::identity<glm::mat4>(); //glm::translate(viewMatrix, glm::vec3(1.f));
	if (yoffset == 1)
	{
		window->Scale(1.05f, 1.05);
		//viewMatrix  = glm::scale(viewMatrix, glm::vec3(1.05f, 1.05f, 1.f));
	}
	else
	{
		window->Scale(0.95, 0.95);
		//viewMatrix = glm::scale(viewMatrix, glm::vec3(0.95, 0.95f, 1.f));
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	GWindow2d* win = GWindow2dManger::Instanse()->GetWindow(window);
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
	{
		win->ResetTransform();
		//GWindow2dManger::Instanse()->GetWindow(window)->ResetTransform();
		//viewMatrix *= glm::inverse(viewMatrix);
	}
}

void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	auto win = GWindow2dManger::Instanse()->GetWindow(window);
	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
	{
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		//std::cout << "GLFW_MOUSE_BUTTON_1 GLFW_PRESSED:\t" << x << "\t" << y << std::endl;
		isMouseBnt1Presed = true;
	}

	if (button == GLFW_MOUSE_BUTTON_3 && action == GLFW_PRESS)
	{

	}

	if (button == GLFW_MOUSE_BUTTON_3 && action == GLFW_RELEASE)
	{

	}
	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE)
	{

	    isMouseBnt1Presed = false;
		//std::cout << "GLFW_MOUSE_BUTTON_1 GLFW_RELEASE:\t" <</* cur_x_pos << "\t" << cur_y_pos << */std::endl;
	}
	
}

void cursor_moved(GLFWwindow* window, double xpos, double ypos)
{
	auto win = GWindow2dManger::Instanse()->GetWindow(window);
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	glm::vec3 newPos(x,y,0);
	double newX, newY;
	
	//glm::vec3 newPos = win->Unproject(x, y);
	if (isMouseBnt1Presed)
	{
		auto curPos = win->GetCurMousePos();
		auto translate = newPos - curPos;
		win->Move(translate);
	}
	
	win->SetMouseCoorditane(newPos);
	
}

void on_resize(GLFWwindow* window, int width, int height)
{

	//projectionMatrix = glm::ortho(0.f, (float)width, (float)height, 0.f, 0.f, 1.f);
}

#pragma endregion


GWindow2d::GWindow2d(size_t width, size_t height, std::string_view title) :
	width_(width), height_(height),title_(title)
{
	
	if (!glfwInit())
		throw std::runtime_error("Could not initialize GLFW");

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	
	window_ = glfwCreateWindow(static_cast<int>(width_), static_cast<int>(height_), title_.c_str(), NULL, NULL);
	UpdateProjection((float)width_, (float)height_);

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

	glfwSetKeyCallback(window_, key_callback);
	glfwSetCursorPosCallback(window_, cursor_moved);
	glfwSetMouseButtonCallback(window_, mouse_callback);
	glfwSetScrollCallback(window_, scroled);
	glfwSetWindowSizeCallback(window_, on_resize);
	viewPort_ = { 0, 0, width_, height_ };
	//glViewport(viewPort_[0], viewPort_[1], viewPort_[2], viewPort_[3]);
}

bool GWindow2d::IsShouldClose() const
{
	return glfwWindowShouldClose(window_);
}

void GWindow2d::SwapBuffer()
{
	glfwSwapBuffers(window_);
	
}

Common::Graphic::GraphicElementPtr GWindow2d::AddGraphicElement(const Common::Graphic::GraphicElementPtr& element)
{
	context_.Add(element);
	return element;
}

glm::mat4 GWindow2d::GetProjectionMatrix() const
{
	return projectionMatrix_;
}

glm::mat4 GWindow2d::GetModelMatrix() const
{
	return model_;
}

glm::mat4 GWindow2d::GetViewMatrix() const
{
	return viewMatrix_;
}

glm::vec4 GWindow2d::GetViewPort() const
{
	return viewPort_;
}

void GWindow2d::SetShader(Common::Resources::ShaderProgramPtr shader)
{
	shader_ = shader;
}

Common::Resources::ShaderProgramPtr GWindow2d::GetSahder()
{
	return shader_;
}

glm::vec3 GWindow2d::Unproject(const glm::vec3& vec) const
{
	return glm::unProject(vec, model_, projectionMatrix_, viewPort_);
}

glm::vec3 GWindow2d::Unproject(float x, float y, float z) const
{
	return Unproject({ x, y, z });
}

glm::vec3 GWindow2d::Unproject(float x, float y) const
{
	return Unproject(x, y, 0);
}

GLFWwindow* GWindow2d::Handle() const
{
	return window_;
}

void GWindow2d::Scale(double xoff, double yoff)
{
	viewMatrix_ = glm::scale(viewMatrix_, glm::vec3(xoff, yoff, 0.f));
	//print(viewMatrix_);
	//viewMatrix_ = glm::translate(viewMatrix_, { vec2[0],vec2[1],vec[2]});
	//projectionMatrix_  = glm::scale(projectionMatrix_, glm::vec3(xoff, yoff, 0.f));
	/*
	context_.ForEach([=](GraphicElementPtr obj) 
		{
			obj->Scale((float)xoff, (float)yoff, 0.f); 
		});*/
}

void GWindow2d::Rotate(double angle, double xoff, double yoff)
{
	//context_.ForEach([=](GraphicElementPtr obj)
	//	{
	//		obj->Rotate((float)angle,(float)xoff, (float)yoff, 0.f);
	//	});
}

void GWindow2d::Move(double xoff, double yoff)
{
	viewMatrix_ = glm::translate(viewMatrix_, glm::vec3(xoff, yoff, 0.f));
}

void GWindow2d::Move(const glm::vec3& vec)
{
	Move(vec[0], vec[1]);
}

void GWindow2d::SetMouseCallBack(MouseCallBack function)
{
	mouseCallBack_ = function;
}

void GWindow2d::SetMouseMoveCallBack(MouseMoveCallBack function)
{
	mouseMoveCallBack_ = function;
}

GWindow2d::MouseCallBack GWindow2d::GetMouseCallBack() const
{
	return mouseCallBack_;
}

GWindow2d::MouseMoveCallBack GWindow2d::GetMouseMoveCallBack() const
{
	return mouseMoveCallBack_;
}

glm::vec3 GWindow2d::GetCurMousePos() const
{
	return curMousePos_;
}

glm::vec3 GWindow2d::SetMouseCoorditane(double x, double y)
{
	return SetMouseCoorditane({ x,y,0 });
}

glm::vec3 GWindow2d::SetMouseCoorditane(const glm::vec3& vec)
{
	curMousePos_ = vec;
	return curMousePos_;
}

void GWindow2d::RegisterWindow() const
{
}

void GWindow2d::ResetTransform()
{
	viewMatrix_ = glm::identity<glm::mat4>();
}

void GWindow2d::Draw()
{
	OnDraw();
}

GWindow2dManger* GWindow2d::Manadger()
{
	return GWindow2dManger::Instanse();
}

float GWindow2d::GetWidth() const
{
	return (float)width_;
}

float GWindow2d::GetHeight() const
{
	return (float)height_;
}

void GWindow2d::UpdateProjection(float width, float height)
{
	// projectionMatrix *= glm::ortho(-(float)WIDTH / 2.f, (float)WIDTH / 2.f, -(float)WIDTH / 2.f, (float)WIDTH / 2.f, -1.f, 1.f);
	projectionMatrix_ = glm::ortho(0.f, width, height, 0.f, 0.f, 1.f);
}

void GWindow2d::DrawAxis()
{
	static GLfloat colors2[] = { 1.0f,0.f,0.f };
	float width = static_cast<float>(width_);
	float height = static_cast<float>(height_);
	GVertexBuffer vertexes;
	float center_x = width / 2;
	float center_y = height / 2;
	float x_vec = center_x + width_ / 10;
	float y_vec = center_y - height / 10;

	glPointSize(5);
	std::vector <Common::Graphic::Point2D> points
	{
		{center_x,center_y,0.f}, {x_vec,center_y,0},
		{center_x,center_y,0.f}, {center_x,y_vec,0},
	};

	GLuint vao;
	glGenVertexArrays(1, &vao);
	vertexes.Atach(points.data(), (GLsizei)(points.size() * sizeof(points[0])));
	glBindVertexArray(vao);
	vertexes.Bind();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_LINES, 0, (GLsizei)(points.size()));

	vertexes.Unbind();
	glBindVertexArray(0);

}

void GWindow2d::DrawGrid()
{
	static GLfloat colors2[] = { 0.0f,0.f,0.f };

	size_t dx = 50;
	size_t dy = 50;

}

void GWindow2d::OnDraw()
{
	if (shader_ != nullptr)
	{
		if (!shader_->IsUsed())
			shader_->Use();

		shader_->SetMatrix4("projection", projectionMatrix_);

		glm::mat4 model = glm::identity<glm::mat4>();
		shader_->SetMatrix4("view", viewMatrix_);
		shader_->SetMatrix4("model", model);
		DrawAxis();
		context_.Render();
		shader_->Unuse();
	}
}
