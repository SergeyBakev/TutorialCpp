#include "stdafx.h"
#include "Window2dManager.h"

GWindow2dManger* GWindow2dManger::Instanse()
{
	static GWindow2dManger instanse;
	return &instanse;
}

bool GWindow2dManger::Register(GWindow2d* window, GLFWwindow* handle)
{
	const auto& it = handle_to_window.find(handle);
	if (it == std::end(handle_to_window))
		return false;

	handle_to_window.insert({ handle ,window });
	window_to_handle.insert({ window,handle });

	return true;
}

GWindow2d* GWindow2dManger::GetWindow(GLFWwindow* handle) const
{
	return handle_to_window.at(handle);
}
