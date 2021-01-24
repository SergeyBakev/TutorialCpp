#pragma once
struct GLFWwindow;
class GWindow2dManger;

class GWindow2d
{
public:
	GWindow2d() = delete;
	GWindow2d(size_t width, size_t height, std::string_view title);


	bool IsShouldClose() const;
	void SwapBuffer() const;
	GLFWwindow* Handle() const;
	void Scale(double xoff, double yoff);

	void RegisterWindow() const;



	static GWindow2dManger* Manadger();



private:

	GLFWwindow* window_;
	double scale_x_ = 1.0;
	double scale_y_ = 1.0;
	size_t width_;
	size_t height_;
	std::string title_;
};