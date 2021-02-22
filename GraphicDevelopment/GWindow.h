#pragma once
struct GLFWwindow;
class GWindow2dManger;

class GWindow
{
public:
	using MouseCallBack = std::function<void(GWindow*, int button, int action, int mods)>;
	using MouseMoveCallBack = std::function<void(GWindow*, double xpos, double ypos)>;
	GWindow() = delete;
	GWindow(size_t width, size_t height, std::string_view title);

	bool IsShouldClose() const;
	void SwapBuffer();

	Common::Graphic::GraphicElementPtr AddGraphicElement(const Common::Graphic::GraphicElementPtr& element);

	glm::mat4 GetProjectionMatrix() const;
	glm::mat4 GetModelMatrix() const;
	glm::mat4 GetViewMatrix() const;
	glm::vec4 GetViewPort() const;

	void SetShader(Common::Resources::ShaderProgramPtr shader_);
	Common::Resources::ShaderProgramPtr GetSahder();

	glm::vec3 Unproject(const glm::vec3& vec) const;
	glm::vec3 Unproject(float x, float y, float z) const;
	glm::vec3 Unproject(float x, float y) const;

	glm::vec3 Project(const glm::vec3& vec) const;
	glm::vec3 Project(float x, float y, float z) const;
	glm::vec3 Project(float x, float y) const;


	GLFWwindow* Handle() const;

	void MainLoop();

	void Scale(double xoff, double yoff);
	
	void Rotate(double xoff,double yoff, double zoff);
	void Rotate(const glm::vec3& vec);
	void Move(double xoff, double yoff);
	void Move(const glm::vec3& vec);
	

	void SetMouseCallBack(MouseCallBack function);
	void SetMouseMoveCallBack(MouseMoveCallBack function);

	MouseCallBack GetMouseCallBack() const;
	MouseMoveCallBack GetMouseMoveCallBack() const;


	glm::vec3 GetCurMousePos() const;
	glm::vec3 GetUnprojCurMousePos() const;
	glm::vec3 SetMouseCoorditane(double x, double y);
	glm::vec3 SetMouseCoorditane(const glm::vec3& vec);
	void RegisterWindow() const;

	void ResetTransform();

	void Draw();

	static GWindow2dManger* Manadger();

	float GetWidth() const;
	float GetHeight() const;
	Common::GBoundingBox GetBbox() const;

	void ZoomAll();
	void ZoomIn(const Common::GBoundingBox& bbox);
	void SetSpaceSize(float size);
	float GetSpaceSize() const;

	void Update();
protected:
	
	virtual bool OnRotate(const glm::vec3& p, const glm::vec3& v);

	virtual bool OnUpdateSizeSpace();
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
	
	float spaceSize_ = 1;
	double scale_x_ = 1.0;
	double scale_y_ = 1.0;
	size_t width_;
	size_t height_;
	std::string title_;

	glm::vec3 curMousePos_;
	glm::mat4 projectionMatrix_ = glm::identity<glm::mat4>();
	glm::mat4 viewMatrix_ = glm::identity<glm::mat4>();
	glm::mat4 model_ = glm::identity<glm::mat4>();
	glm::vec4 viewPort_;
};