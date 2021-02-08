#include "stdafx.h"
#include "GL/GCContext.h"
#include "Window/GWindow.h"
#include "Window/Window2dManager.h"
#include "Resources/ResourceManager.h"
#include "Resources/ShaderProgram.h"
#include "Mathematic/GMathematic.h"
#include "GL/GPoint.h"
#include "GL/Graphic.h"
#include "GL/GPolygon.h"
#include "GL/g_detail.h"
#include "GL/GVertexBufferObject.h"
#include "GL/GIndexArrayObject.h"
#include "GL/Graphic.h"



void print(glm::mat4& m)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            std::cout << m[i][j] << "\t\t";
        }
        std::cout << std::endl;
    }
}

void print(glm::vec4& vec)
{
    for (int i = 0; i < 4; i++)
    {
        std::cout << vec[i] << "\t\t";
    }
}
void print(glm::vec3& vec)
{
    for (int i = 0; i < 3; i++)
    {
        std::cout << vec[i] << "\t\t";
    }
}