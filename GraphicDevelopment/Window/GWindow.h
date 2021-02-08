#pragma once
#include "..\GL\GCContext.h"
#include "..\Resources\ShaderProgram.h"
struct GLFWwindow;
class GWindow2dManger;

class GWindow2d
{
public:
	using MouseCallBack = std::function<void(GWindow2d*, int button, int action, int mods)>;
	using MouseMoveCallBack = std::function<void(GWindow2d*, double xpos, double ypos)>;
	GWindow2d() = delete;
	GWindow2d(size_t width, size_t height, std::string_view title);

	bool IsShouldClose() const;
	void SwapBuffer();

	Common::Graphic::GraphicElementPtr AddGraphicElement(const Common::Graphic::GraphicElementPtr& element);

	glm::mat4 GetProjectionMatrix() const;
	glm::mat4 GetModelMatrix() const;
	glm::mat4 GetViewMatrix() const;
	glm::vec4 GetViewPort() const;

	void SetShader(Common::Resources::ShaderProgramPtr shader_);
	Common::Resources::ShaderProgramPtr GetSahder();

	GLFWwindow* Handle() const;

	void Scale(double xoff, double yoff);
	
	void Rotate(double angle,double xoff, double yoff);
	void Move(double xoff, double yoff);
	void Move(const glm::vec3& vec);

	void SetMouseCallBack(MouseCallBack function);
	void SetMouseMoveCallBack(MouseMoveCallBack function);

	MouseCallBack GetMouseCallBack() const;
	MouseMoveCallBack GetMouseMoveCallBack() const;

	void RegisterWindow() const;

	void ResetTransform();

	void Draw();

	static GWindow2dManger* Manadger();

	float GetWidth() const;
	float GetHeight() const;

private:
	void UpdateProjection(float width, float height);

	void DrawAxis();
	void DrawGrid();

	void OnDraw();
private: 
	//CallBack
	MouseCallBack mouseCallBack_;
	MouseMoveCallBack mouseMoveCallBack_;

private:
	GLFWwindow* window_;
	Common::Graphic::GCContext context_;
	Common::Resources::ShaderProgramPtr shader_;
	
	double scale_x_ = 1.0;
	double scale_y_ = 1.0;
	size_t width_;
	size_t height_;
	std::string title_;


	glm::mat4 projectionMatrix_ = glm::identity<glm::mat4>();
	glm::mat4 viewMatrix_ = glm::identity<glm::mat4>();
	glm::mat4 model_ = glm::identity<glm::mat4>();
	glm::vec4 viewPort_;
};