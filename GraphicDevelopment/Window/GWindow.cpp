#include "stdafx.h"
#include "GWindow.h"
#include "..\Resources\ResourceManager.h"
#include "Window2dManager.h"

using namespace Common::Graphic;
using namespace Common::Resources;

void print(glm::mat4& m);
float fovy = 45;
float scale = 1.1f;
double cur_x_pos;
double cur_y_pos;
bool isMousePresed = false;
bool isMouse3Presed = false;

#pragma region Window callback

void scroled(GLFWwindow* win, double xoffset, double yoffset)
{
	GWindow2d* window = GWindow2dManger::Instanse()->GetWindow(win);
	auto mat = glm::identity<glm::mat4>(); //glm::translate(viewMatrix, glm::vec3(1.f));
	if (yoffset == 1)
	{
		//window->Scale(1.05f, 1.05);
		//window->ScaleObjects(1.05f, 1.05f);
		//viewMatrix  = glm::scale(viewMatrix, glm::vec3(1.05f, 1.05f, 1.f));
	}
	else
	{
		//window->Scale(0.95, 0.95);
		//viewMatrix = glm::scale(viewMatrix, glm::vec3(0.95, 0.95f, 1.f));
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_F5 && action == GLFW_PRESS)
	{
		GWindow2dManger::Instanse()->GetWindow(window)->ResetTransform();
		//viewMatrix *= glm::inverse(viewMatrix);
	}
}

void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
	{
		isMousePresed = true;
		glfwGetCursorPos(window, &cur_x_pos, &cur_y_pos);
		std::cout << "Mouse1 presed\n";
		//std::cout << cur_x_pos << "\t" << cur_x_pos << std::endl;
		//scene.Add(std::make_shared<GPoint2D>(v));
	}

	if (button == GLFW_MOUSE_BUTTON_3 && action == GLFW_PRESS)
	{
		isMouse3Presed = true;
	}

	if (button == GLFW_MOUSE_BUTTON_3 && action == GLFW_RELEASE)
	{
		isMouse3Presed = false;
	}
	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE)
	{
		isMousePresed = false;
		std::cout << "Mouse1 realesed\n";
	}
}

void cursor_moved(GLFWwindow* win, double xpos, double ypos)
{
	if (isMousePresed && !isMouse3Presed)
	{
		glm::vec3 v(-xpos / 100, -ypos / 100, 0.f);
		std::cout << xpos << "\t" << ypos << std::endl;
		double xoffset = (cur_x_pos - xpos) / -30;
		double yoffset = (cur_y_pos - ypos) / 30;
		int z = 3;
		//viewMatrix = glm::translate(viewMatrix, glm::vec3(xoffset, yoffset, 0.f));
	}
	if (!isMousePresed && isMouse3Presed)
	{
		/* glm::vec3 v(-xpos / 100, -ypos / 100, 0.f);
		 std::cout << xpos << "\t" << ypos << std::endl;
		 double xoffset = (cur_x_pos - xpos) / -30;
		 double yoffset = (cur_y_pos - ypos) / 30;
		 viewMatrix = glm::rotate(viewMatrix,15.f, glm::vec3(0.1, 0.1, 0.f));*/
		GWindow2d* window = GWindow2dManger::Instanse()->GetWindow(win);
		//window->RotateObjects(15, 0, 0);
	}
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
	//UpdateProjection((float)width_, (float)height_);

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

void GWindow2d::Scale(double xoff, double yoff)
{
	viewMatrix_ = glm::scale(viewMatrix_, glm::vec3(xoff, yoff, 0.f));

	projectionMatrix_  = glm::scale(projectionMatrix_, glm::vec3(xoff, yoff, 0.f));
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
	static GLfloat colors2[] = { 1.0f,0.f,0.f };
	float width = static_cast<float>(width_);
	float height = static_cast<float>(height_);
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vertexbuffer;
	GLuint colors_vbo = 0;
	GLfloat xAxis[]
	{
		0.f,height /2, 0.f,
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

	glDrawArrays(GL_POLYGON, 0, 4);
}

void GWindow2d::DrawGrid()
{
	static GLfloat colors2[] = { 0.0f,0.f,0.f };

	size_t dx = 50;
	size_t dy = 50;
	std::vector<glm::vec3> points;
	//points.push_back(glm::vec3(0, 0, 0.f));
	//points.push_back(glm::vec3(0, 50, 0.f));
	//points.push_back(glm::vec3(50, 50, 0.f));
	//points.push_back(glm::vec3(50, 0, 0.f));

	//glClear(GL_COLOR_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	int xsize = 0, ysize = 0;
	for (int j = 0; j < 10; j++)
	{

		xsize = 0;
		for (int i = 0; i < 10; i++)
		{
			points.push_back(glm::vec3(-50.0 + xsize, -50.0 + ysize, 0.0));
			points.push_back(glm::vec3(-40.0 + xsize, -50.0 + ysize, 0.0));
			points.push_back(glm::vec3(-40.0 + xsize, -40.0 + ysize, 0.0));
			points.push_back(glm::vec3(-50.0 + xsize, -40.0 + ysize, 0.0));

			xsize += 10.0;
		}
		ysize += 10.0;
	}

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vertexbuffer;
	GLuint colors_vbo = 0;

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), points.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors2), &colors2, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, nullptr);


	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLineWidth(1);
	glDrawArrays(GL_POLYGON, 0, (GLsizei)points.size());
}

void GWindow2d::OnDraw()
{
	shader_->Use();
	shader_->SetMatrix4("projection", projectionMatrix_);
	glm::mat4 model = glm::identity<glm::mat4>();
	shader_->SetMatrix4("model", model);
	shader_->SetMatrix4("view", viewMatrix_);
	//DrawGrid();
	//context_.Render();
	shader_->Unuse();
}
