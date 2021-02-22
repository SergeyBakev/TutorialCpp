#include "stdafx.h"
#include "Window2dManager.h"

GWindow2dManger* GWindow2dManger::Instanse()
{
	static GWindow2dManger instanse;
	return &instanse;
}

bool GWindow2dManger::Register(GWindow* window, GLFWwindow* handle)
{
	const auto& it = handle_to_window.find(handle);
	if (it != std::end(handle_to_window))
		return false;

	handle_to_window.insert({ handle ,window });
	window_to_handle.insert({ window,handle });

	return true;
}

bool GWindow2dManger::RegisterByName(std::string name, GWindow* window)
{
	const auto& it = name_to_window.find(name);
	if (it != std::end(name_to_window))
		return false;

	name_to_window.insert({ name,window });
	return true;
}

GWindow* GWindow2dManger::GetWindow(GLFWwindow* handle) const
{
	return handle_to_window.at(handle);
}

GWindow* GWindow2dManger::GetWindowByName(std::string str) const
{
	return name_to_window.at(str);
}
