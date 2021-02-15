// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"
#define GLEW_STATIC
#include <GL/glew.h>
#include <Gl/GLU.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include "..\Include\Eigen\Dense"
#include <vector>
#include <stdexcept>
#include <Windows.h>
#include <thread>
#include <chrono>
#include <fstream>
#include <sstream>
#include <math.h>
#include <iostream>
#include <string_view>
#include <array>
#include <algorithm>
#include <functional>

using glm3Vectors = std::vector<glm::vec3>;
#endif //PCH_H
