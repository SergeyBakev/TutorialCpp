#include "stdafx.h"
#include "GWindow.h"
#include "..\Resources\ResourceManager.h"
#include "Window2dManager.h"

using namespace Common::Graphic;
using namespace Common::Resources;

void print(glm::mat4& m);

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
	//glViewport(0, 0, width_, height_);
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

glm::mat4 GWindow2d::GetProjectionMatrix()
{
	return projectionMatrix_;
}

void GWindow2d::SetShader(Common::Resources::ShaderProgramPtr shader)
{
	shader_ = shader;
}

GLFWwindow* GWindow2d::Handle() const
{
	return window_;
}

void GWindow2d::ScaleObjects(double xoff, double yoff)
{
	context_.ForEach([=](GraphicElementPtr obj) 
		{
			obj->Scale((float)xoff, (float)yoff, 0.f); 
		});
}

void GWindow2d::RotateObjects(double angle, double xoff, double yoff)
{
	context_.ForEach([=](GraphicElementPtr obj)
		{
			obj->Rotate((float)angle,(float)xoff, (float)yoff, 0.f);
		});
}

void GWindow2d::RegisterWindow() const
{
}

void GWindow2d::ResetTransform()
{
	context_.ForEach([=](GraphicElementPtr obj)
		{
			auto model = obj->GetTransofrm();
			model[2][2] = 1;		
			model = glm::inverse(model);
			model[2][2] = 0;
			obj->MultMatrix(model);
		});
}

void GWindow2d::Draw()
{
	OnDraw();
}

GWindow2dManger* GWindow2d::Manadger()
{
	return GWindow2dManger::Instanse();
}

void GWindow2d::UpdateProjection(float width, float height)
{
	// projectionMatrix *= glm::ortho(-(float)WIDTH / 2.f, (float)WIDTH / 2.f, -(float)WIDTH / 2.f, (float)WIDTH / 2.f, -1.f, 1.f);
	projectionMatrix_ = glm::ortho(0.f, width, height, 0.f, 0.f, 1.f);
}

void GWindow2d::DrawAxis()
{
	static GLfloat colors2[] = { 0.0f,0.f,0.f };
	float width = static_cast<float>(width_);
	float height = static_cast<float>(height_);
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vertexbuffer;
	GLuint colors_vbo = 0;
	GLfloat xAxis[]
	{
		0.f,height /2,0.f,
		width, height /2 , 0.f,
		width / 2, 0.f,0.f,
		width / 2, height,0.f,
	};

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(xAxis), xAxis, GL_STATIC_DRAW);

	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors2), &colors2, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, nullptr);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_LINES, 0, 4);
}

void GWindow2d::OnDraw()
{
	
	if (shader_)
		shader_->SetMatrix4("projection", projectionMatrix_);
	glm::mat4 model = glm::identity<glm::mat4>();
	shader_->SetMatrix4("model", model);
	shader_->Use();

	DrawAxis();
	context_.Render();
	//shader_->Unuse();
}
