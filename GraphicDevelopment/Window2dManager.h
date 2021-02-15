#pragma once
class GWindow2d;
struct GLFWwindow;
class GWindow2dManger
{
public:

	static GWindow2dManger* Instanse();

	bool Register(GWindow2d* window, GLFWwindow* handle);
	bool RegisterByName(std::string name, GWindow2d* window);
	GWindow2d* GetWindow(GLFWwindow* handle) const;
	GWindow2d* GetWindowByName(std::string str) const;

private:
	std::map< GLFWwindow*, GWindow2d*> handle_to_window;
	std::map< std::string, GWindow2d*> name_to_window;
	std::map< GWindow2d*, GLFWwindow*> window_to_handle;
};