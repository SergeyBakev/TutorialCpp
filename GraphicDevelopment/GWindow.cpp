#include "stdafx.h"
#include "GWindow.h"
#include "Window2dManager.h"

using namespace Common::Graphic;
using namespace Common::GeometryPrimitive;
using namespace Common;
using namespace Common::Resources;

//#define DEBUG_ROTATE

void print(glm::mat4& m);
void print(glm::vec3& m);
void print(glm::vec4& m);

#ifdef DEBUG_ROTATE
GraphicElementPtr dragVector;
GraphicElementPtr toStartpoint;
GraphicElementPtr toEndpoint;
GraphicElementPtr tickPoint;
GraphicElementPtr startTickPoint;
GraphicElementPtr normalVector;
#endif

bool isMouseBnt1Presed = false;
bool isMouseBnt3Presed = false;
bool isMouseCtrlPresed = false;
float fovy = 45;
float scale = 1.1f;

#pragma region Window callback

float amb = 0.1f;
void scroled(GLFWwindow* win, double xoffset, double yoffset)
{
	GWindow* window = GWindow2dManger::Instanse()->GetWindow(win);
	auto mat = glm::identity<glm::mat4>(); //glm::translate(viewMatrix, glm::vec3(1.f));
	if (isMouseCtrlPresed)
	{
		
	}
	else
	{
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
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	GWindow* win = GWindow2dManger::Instanse()->GetWindow(window);
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_HOME && action == GLFW_PRESS)
	{
		win->ResetTransform();
#ifdef DEBUG_ROTATE
		win->RemoveGraphicElement(tickPoint);
		win->RemoveGraphicElement(startTickPoint);
		win->RemoveGraphicElement(dragVector);
		win->RemoveGraphicElement(toStartpoint);
		win->RemoveGraphicElement(normalVector);
#endif // 0

		
		//GWindow2dManger::Instanse()->GetWindow(window)->ResetTransform();
		//viewMatrix *= glm::inverse(viewMatrix);
	}
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		win->OnUp(5.f);
	}

	if (key == GLFW_KEY_KP_5 && action == GLFW_PRESS)
	{
		win->OnRotate(5.f, { 0,0,1 });
	}

	if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS)
	{
		isMouseCtrlPresed = true;
	}

	if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_RELEASE)
	{
		isMouseCtrlPresed = false;
	}

	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		win->OnDown(-5.f);
	}

	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		win->OnLeft(-5.f);
	}

	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		win->OnRight(5.f);
	}

	
}

void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	auto win = GWindow2dManger::Instanse()->GetWindow(window);
	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
	{
		//std::cout << "GLFW_MOUSE_BUTTON_1 GLFW_PRESSED:\t" << x << "\t" << y << std::endl;
		auto curMouse = win->GetUnprojCurMousePos();
		
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
	glm::vec3 newPos = win->ToWindowPoint((float)x, (float)y);
	//glm::vec3 newPos = win->Unproject(x, y);
	if (isMouseBnt1Presed && !isMouseBnt3Presed)
	{
		auto curPos = win->GetUnprojCurMousePos();
		auto translate = win->Unproject(newPos) - curPos;
		if (!Common::detial::IsValid(translate))
			return;
		win->Move(translate[0],translate[1]);

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

#pragma region GWindow implementation

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

	glDisable(GL_LIGHTING);
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
	_ASSERT_EXPR(element != nullptr, L"GWindow::AddGraphicElemen fail. ElementPtr shoul be not nullptr");
	context_.Add(element);
	Update();
	return element;
}

Common::Graphic::GraphicElementPtr GWindow::RemoveGraphicElement(const Common::Graphic::GraphicElementPtr& element)
{
	auto el =  context_.Remove(element);
	Update();
	return el;
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
	return glm::unProject(vec, viewMatrix_ * model_ , projectionMatrix_, viewPort_);
}

glm::vec3 GWindow::Project(const glm::vec3& vec) const
{
	return glm::project(vec, viewMatrix_ * model_, projectionMatrix_, viewPort_);
}

glm::vec3 GWindow::Unproject(float x, float y, float z) const
{
	return Unproject({ x, y, z });
}

glm::vec3 GWindow::Unproject(float x, float y) const
{
	return Unproject(x, y, 0);
}

glm::vec3 GWindow::Project(float x, float y, float z) const
{
	return Project({ x,y,z });
}

glm::vec3 GWindow::Project(float x, float y) const
{
	return Project({ x,y,0 });
}

glm::vec3 GWindow::ToWindowPoint(const glm::vec3& viewPoint) const
{
	return { viewPoint.x, height_ - viewPoint.y,0 };
}

glm::vec3 GWindow::ToWindowPoint(float x, float y) const
{
	return ToWindowPoint({ x,y,0 });
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
}

void GWindow::Rotate(double xoff, double yoff, double zoff)
{
	Rotate({ xoff,yoff,zoff });
}
void GWindow::Rotate(const glm::vec3& vec)
{
	auto p0 = GetUnprojCurMousePos();
	auto p1 = Unproject(vec);
#ifdef DEBUG_ROTATE
	{
		if (tickPoint != nullptr)
			RemoveGraphicElement(tickPoint);

		if (startTickPoint != nullptr)
			RemoveGraphicElement(startTickPoint);

		if (toStartpoint != nullptr)
			RemoveGraphicElement(toStartpoint);

		if (toEndpoint != nullptr)
			RemoveGraphicElement(toEndpoint);

		if (normalVector != nullptr)
			RemoveGraphicElement(normalVector);

		startTickPoint = G_MAKE(GPoint)(p0);
		startTickPoint->SetSize(5).SetColor({ 1,0.5,1 });
		AddGraphicElement(startTickPoint);

		tickPoint = G_MAKE(GPoint)(p1);
		tickPoint->SetSize(5).SetColor({ 1,0.5,1 });
		AddGraphicElement(tickPoint);
	}
#endif
	glm::vec3 v = p1 - p0;
	if (!Common::detial::IsValid(v))
		return;

	OnRotate(p0, v);
}

void GWindow::Rotate(float angle, const glm::vec3& axis)
{
	viewMatrix_ = glm::rotate(viewMatrix_,angle, axis);
}

void GWindow::Move(double xoff, double yoff)
{
	viewMatrix_ = glm::translate(viewMatrix_, glm::vec3(xoff, yoff, 0.f));
}

void GWindow::Move(const glm::vec3& vec)
{
	Move(vec[0], vec[1]);
}

void GWindow::OnUp(float angle)
{
	OnRotate(glm::radians(angle), glm::vec3(1, 0, 0));
}

void GWindow::OnDown(float angle)
{
	OnRotate(glm::radians(angle), glm::vec3(1, 0, 0));
}

void GWindow::OnLeft(float angle)
{
	OnRotate(glm::radians(angle), glm::vec3(0, 1, 0));
}

void GWindow::OnRight(float angle)
{

	OnRotate(glm::radians(angle), glm::vec3(0, 1, 0));
}

bool GWindow::OnRotate(float angle, const glm::vec3& axis)
{
	viewMatrix_ = glm::rotate(viewMatrix_,angle, axis);
	return true;
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
	//curMousePos_.y = -curMousePos_.y;
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

LightLamp& GWindow::GetLamp()
{
	return lamp_;
}


bool GWindow::OnRotate(const glm::vec3& p0, const glm::vec3& v)
{
	glm::vec3 test = { 397,442,0 };
	auto vec = Unproject(test);
	auto m_DimModel = context_.GetBBox();
	if (!m_DimModel.IsValid())
		return false;

	glm::vec3 pc = m_DimModel.Center();
	float r = glm::length(glm::vec3(m_DimModel.Max() - pc));
	glm::vec3 v1 = (p0 - pc);
	float Lv1 = glm::length(v1);
	v1 *= r / Lv1;

	glm::vec3 norm = glm::cross(v1, v);
	glm::vec3 v2 = v1 + v;

	norm = glm::normalize(norm);
	v1 = glm::normalize(v1);
	v2 = glm::normalize(v2);


	float cosg = glm::dot(v1, v2);
	float angle = acos(cosg);
	Move(pc);
	Rotate(angle, norm);
	Move(-pc);

#ifdef DEBUG_ROTATE
	normalVector = G_MAKE(GLine)(Line(p0, norm));
	normalVector->SetSize(4).SetColor(BLACK);
	AddGraphicElement(normalVector);

	toStartpoint = G_MAKE(GLine)(Line(pc, v1));
	toStartpoint->SetSize(4).SetColor(BLACK);
	AddGraphicElement(toStartpoint);

	/*toEndpoint = G_MAKE(GLine)(Line(p0, pc));
	toEndpoint->SetSize(4).SetColor(BLACK);
	AddGraphicElement(toEndpoint);*/
#endif


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

	UpdateProjection(rw, rh,rz);

	return true;
}

void GWindow::UpdateProjection(float width, float height, float zFar)
{
	projectionMatrix_ = glm::ortho(-width, width, -height, height,-zFar, zFar);
	//projectionMatrix_ = glm::ortho(-1, 1, -1, 1,-1, 1);
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
		shader_->SetUniform3f("lightColor", lamp_.GetColor());
		shader_->SetUniformf("ambientStrength", lamp_.GetAmbientStrength());
		glm::mat4 model = glm::identity<glm::mat4>();
		shader_->SetUniformMatrix4("model", model);
		//DrawAxis();
		context_.Render();
		shader_->Unuse();
	}
}

#pragma endregion