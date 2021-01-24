#include "stdafx.h"
#include "GCContext.h"


GCContext::GCContext()
{ /* Initialize the library */
    if (!glfwInit())
        throw std::runtime_error("Could not initialize GLFW");
    if(glewInit() != GLEW_OK)
        throw std::runtime_error("Could not initialize GLEW");
}

GCContext::~GCContext()
{
    glfwTerminate();
    glewExperimental = true;
}
