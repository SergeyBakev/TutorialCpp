#pragma once
class GWindow;
struct GLFWwindow;
class GWindow2dManger
{
public:

	static GWindow2dManger* Instanse();

	bool Register(GWindow* window, GLFWwindow* handle);
	bool RegisterByName(std::string name, GWindow* window);
	GWindow* GetWindow(GLFWwindow* handle) const;
	GWindow* GetWindowByName(std::string str) const;

private:
	std::map< GLFWwindow*, GWindow*> handle_to_window;
	std::map< std::string, GWindow*> name_to_window;
	std::map< GWindow*, GLFWwindow*> window_to_handle;
};