#pragma once
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

#include "GIndexArrayObject.h"
#include "GVertexBufferObject.h"
#include "Graphic.h"
#include "GPoint.h"
#include "GPolygon.h"
#include "GCContext.h"
#include "GMathematic.h"
#include "ResourceManager.h"
#include "ShaderProgram.h"
#include "GBoundingBox.h"
#include "ge_validation.h"
#include "def_gl_object.h"
#include "ge_utils.h"
#include "GVector3d.h"
#include "ge_vec.h"
#include "Circle.h"
#include "Arc.h"
#include "Line.h"
#include "GLine.h"
#include "GCircle.h"
#include "ge_color.h"

using glm3Vectors = std::vector<glm::vec3>;