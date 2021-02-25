#include "stdafx.h"
#include "GWindow.h"
#include "Window2dManager.h"

using namespace Common::Graphic;
using namespace Common;
using namespace Common::Resources;

void print(glm::mat4& m);
void print(glm::vec3& m);
void print(glm::vec4& m);

bool isMouseBnt1Presed = false;
bool isMouseBnt3Presed = false;
float fovy = 45;
float scale = 1.1f;

#pragma region Window callback

void scroled(GLFWwindow* win, double xoffset, double yoffset)
{
	GWindow* window = GWindow2dManger::Instanse()->GetWindow(win);
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
	GWindow* win = GWindow2dManger::Instanse()->GetWindow(window);
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_HOME && action == GLFW_PRESS)
	{
		win->ResetTransform();
		//GWindow2dManger::Instanse()->GetWindow(window)->ResetTransform();
		//viewMatrix *= glm::inverse(viewMatrix);
	}
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		win->Rotate(20.f,glm::vec3(0,1,1));
	}

	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{

	}

	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		
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
		isMouseBnt3Presed = true;
	}

	if (button == GLFW_MOUSE_BUTTON_3 && action == GLFW_RELEASE)
	{
		isMouseBnt3Presed = false;
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
	
	//glm::vec3 newPos = win->Unproject(x, y);
	if (isMouseBnt1Presed && !isMouseBnt3Presed)
	{
		auto curPos = win->GetUnprojCurMousePos();
		auto translate = win->Unproject(newPos) - curPos;
		if (!Common::detial::IsValid(translate))
			return;
		win->Move(translate[0],-translate[1]);

	}

	if (isMouseBnt3Presed && !isMouseBnt1Presed)
	{
		win->Rotate(newPos);
	}
	
	win->SetMouseCoorditane(newPos);
	
}

void on_resize(GLFWwindow* window, int width, int height)
{

	//projectionMatrix = glm::ortho(0.f, (float)width, (float)height, 0.f, 0.f, 1.f);
}

#pragma endregion


GWindow::GWindow(size_t width, size_t height, std::string_view title) :
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
	Manadger()->RegisterByName(title.data(),this);

	glfwSetKeyCallback(window_, key_callback);
	glfwSetCursorPosCallback(window_, cursor_moved);
	glfwSetMouseButtonCallback(window_, mouse_callback);
	glfwSetScrollCallback(window_, scroled);
	glfwSetWindowSizeCallback(window_, on_resize);
	viewPort_ = { 0, 0, width_, height_ };
	//glViewport(viewPort_[0], viewPort_[1], viewPort_[2], viewPort_[3]);
}

bool GWindow::IsShouldClose() const
{
	return glfwWindowShouldClose(window_);
}

void GWindow::SwapBuffer()
{
	glfwSwapBuffers(window_);
	
}

Common::Graphic::GraphicElementPtr GWindow::AddGraphicElement(const Common::Graphic::GraphicElementPtr& element)
{
	context_.Add(element);
	Update();
	return element;
}

glm::mat4 GWindow::GetProjectionMatrix() const
{
	return projectionMatrix_;
}

glm::mat4 GWindow::GetModelMatrix() const
{
	return model_;
}

glm::mat4 GWindow::GetViewMatrix() const
{
	return viewMatrix_;
}

glm::vec4 GWindow::GetViewPort() const
{
	return viewPort_;
}

void GWindow::SetShader(Common::Resources::ShaderProgramPtr shader)
{
	shader_ = shader;
}

Common::Resources::ShaderProgramPtr GWindow::GetSahder()
{
	return shader_;
}

glm::vec3 GWindow::Unproject(const glm::vec3& vec) const
{
	return glm::unProject(vec, model_, projectionMatrix_, viewPort_);
}

glm::vec3 GWindow::Unproject(float x, float y, float z) const
{
	return Unproject({ x, y, z });
}

glm::vec3 GWindow::Unproject(float x, float y) const
{
	return Unproject(x, y, 0);
}

glm::vec3 GWindow::Project(const glm::vec3& vec) const
{
	return glm::project(vec, model_, projectionMatrix_, viewPort_);
}

glm::vec3 GWindow::Project(float x, float y, float z) const
{
	return Project({ x,y,z });
}

glm::vec3 GWindow::Project(float x, float y) const
{
	return Project({ x,y,0 });
}

GLFWwindow* GWindow::Handle() const
{
	return window_;
}

void GWindow::MainLoop()
{

}

void GWindow::Scale(double xoff, double yoff)
{
	projectionMatrix_ = glm::scale(projectionMatrix_, glm::vec3(xoff, yoff, 1.f));
	//print(viewMatrix_);
	//viewMatrix_ = glm::translate(viewMatrix_, { vec2[0],vec2[1],vec[2]});
	//projectionMatrix_  = glm::scale(projectionMatrix_, glm::vec3(xoff, yoff, 0.f));
	/*
	context_.ForEach([=](GraphicElementPtr obj) 
		{
			obj->Scale((float)xoff, (float)yoff, 0.f); 
		});*/
}

void GWindow::Rotate(double xoff, double yoff, double zoff)
{
	Rotate({ xoff,yoff,zoff });
}

void GWindow::Rotate(const glm::vec3& vec)
{
	auto p0 = Unproject(GetCurMousePos());
	auto p1 = Unproject(vec);
	
	glm::vec3 v = p1 - p0;
	_ASSERT(Common::detial::IsValid(v));
	OnRotate(p0, v);
}

void GWindow::Rotate(float angle, const glm::vec3& axis)
{
	viewMatrix_ = glm::rotate(glm::radians(angle), axis);
}

void GWindow::Move(double xoff, double yoff)
{
	viewMatrix_ = glm::translate(viewMatrix_, glm::vec3(xoff, yoff, 0.f));
}

void GWindow::Move(const glm::vec3& vec)
{
	Move(vec[0], vec[1]);
}

void GWindow::SetMouseCallBack(MouseCallBack function)
{
	mouseCallBack_ = function;
}

void GWindow::SetMouseMoveCallBack(MouseMoveCallBack function)
{
	mouseMoveCallBack_ = function;
}

GWindow::MouseCallBack GWindow::GetMouseCallBack() const
{
	return mouseCallBack_;
}

GWindow::MouseMoveCallBack GWindow::GetMouseMoveCallBack() const
{
	return mouseMoveCallBack_;
}

glm::vec3 GWindow::GetCurMousePos() const
{
	return curMousePos_;
}

glm::vec3 GWindow::GetUnprojCurMousePos() const
{
	return Unproject(GetCurMousePos());
}

glm::vec3 GWindow::SetMouseCoorditane(double x, double y)
{
	return SetMouseCoorditane({ x,y,0 });
}

glm::vec3 GWindow::SetMouseCoorditane(const glm::vec3& vec)
{
	curMousePos_ = vec;
	return curMousePos_;
}

void GWindow::RegisterWindow() const
{
}

void GWindow::ResetTransform()
{
	Update();
	viewMatrix_ = glm::identity<glm::mat4>();
}

void GWindow::Draw()
{
	OnDraw();
}

GWindow2dManger* GWindow::Manadger()
{
	return GWindow2dManger::Instanse();
}

float GWindow::GetWidth() const
{
	return (float)width_;
}

float GWindow::GetHeight() const
{
	return (float)height_;
}

GBoundingBox GWindow::GetBbox() const
{
	GBoundingBox bbox;
	bbox.Set(Unproject({ viewPort_[0], viewPort_[1], 0 }), true);
	bbox.Set(Unproject({ viewPort_[0] + viewPort_[2] , (viewPort_[1] + viewPort_[3]), 0 }), true);
	return bbox;
}

void GWindow::ZoomAll()
{

	OnUpdateSizeSpace();

	GBoundingBox bbox;

	int i;
	glm3Vectors corners;
	auto bbx = context_.GetBBox();
	context_.GetBBox().GetCorners(corners);

	for (i = 0; i < corners.size(); i++)
	{
		glm::vec3& pnt = corners[i];
		bbox.Set(Project(pnt), true);
	}

	ZoomIn(bbox);

	bbox = GBoundingBox();

	for (i = 0; i < corners.size(); i++)
	{
		glm::vec3& pnt = corners[i];
		bbox.Set(Project(pnt), true);
	}

	ZoomIn(bbox);

}

void GWindow::ZoomIn(const Common::GBoundingBox& bbox)
{
	if (!bbox.IsValid())
		return;

	
	float XRatio = 1.0;
	float YRatio = 1.0;
	auto diagonal = bbox.Diagonal();
	XRatio = (viewPort_[2]) / diagonal.x;
	XRatio = (viewPort_[3]) / diagonal.y;

	auto dim = GetBbox();

	glm::vec3 x = bbox.Center();
	glm::vec3 c0 = dim.Center();
	glm::vec3 c1 = Unproject(x);
	glm::vec3 drag = c0 - c1;
	Move(drag);

	float scale = XRatio < YRatio ? XRatio : YRatio;
	Scale(scale, scale);
}

void GWindow::SetSpaceSize(float size)
{
	spaceSize_ = size;
	OnUpdateSizeSpace();
}

float GWindow::GetSpaceSize() const
{
	return spaceSize_;
}

void GWindow::Update()
{
	auto bbox = context_.GetBBox();
	if (!bbox.IsValid())
	{
		return;
	}
	SetSpaceSize((float)bbox.MaximumDistanceTo({ 0,0,0 }));
}

bool GWindow::OnRotate(const glm::vec3& p, const glm::vec3& v)
{
	auto m_DimModel = context_.GetBBox();
	if (!m_DimModel.IsValid())
		return false;

	glm::vec3 pc = m_DimModel.Center();
	glm::vec3 	vr = (m_DimModel.Max() - pc);
	double r = Common::Length2(vr);
	glm::vec3  v1 = (p - pc);
	double Lv1 =  Common::Length2(v1);
	v1 *= r / Lv1; // нормировка вектора величиной радиуса модели
	auto v2 = v1 + v;

	// определяем вектор, вокруг которого осуществляем вращение
	auto norm = glm::cross(v1, v);

	viewMatrix_ = glm::translate(viewMatrix_, pc);
	Common::Unitize(norm);

	// определяем угол поворота
	Common::Unitize(v1);
	Common::Unitize(v2);

	float cosg = glm::dot(v1,v2);
	float angle = glm::degrees(acos(cosg));
	viewMatrix_ = glm::rotate(viewMatrix_, angle, norm);
	viewMatrix_ = glm::translate(viewMatrix_, -pc);
	
	return true;
}

bool GWindow::OnUpdateSizeSpace()
{
	float cx = viewPort_[2] + viewPort_[0];
	float cy = viewPort_[3] + viewPort_[1];

	float rw, rh, rz;
	rz = rw = rh = GetSpaceSize();

	float fAspect = cx / cy;
	if (cx > cy)
		rw *= fAspect;
	else
		rh /= fAspect;

	UpdateProjection(rw, rh);

	return true;
}

void GWindow::UpdateProjection(float width, float height)
{
	projectionMatrix_ = glm::ortho(-width, width, -height, height);
	// projectionMatrix *= glm::ortho(-(float)WIDTH / 2.f, (float)WIDTH / 2.f, -(float)WIDTH / 2.f, (float)WIDTH / 2.f, -1.f, 1.f);
	//projectionMatrix_ = glm::ortho(0.f, width, height, 0.f, 0.f, 1.f);
	//projectionMatrix_ = glm::inverse(projectionMatrix_);
}

void GWindow::DrawAxis()
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

void GWindow::DrawGrid()
{
	static GLfloat colors2[] = { 0.0f,0.f,0.f };

	size_t dx = 50;
	size_t dy = 50;

}

void GWindow::OnDraw()
{
	if (shader_ != nullptr)
	{
		if (!shader_->IsUsed())
			shader_->Use();

		shader_->SetUniformMatrix4("projection", projectionMatrix_);
		shader_->SetUniformMatrix4("view", viewMatrix_);

		glm::mat4 model = glm::identity<glm::mat4>();
		shader_->SetUniformMatrix4("model", model);
		//DrawAxis();
		context_.Render();
		shader_->Unuse();
	}
}
