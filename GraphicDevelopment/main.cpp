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

using namespace Common::Mathematic;
using namespace Common::Graphic;
using namespace Common::Graphic::detail;
using namespace std::string_literals;
using namespace std::chrono_literals;
using namespace Common::Resources;


#define HEIGHT 800
#define WIDTH  800


GLfloat colors[] = { 0.0f,0.f,0.f };

glm::mat4 scaleMatrix = glm::identity<glm::mat4>();
glm::mat4 modelMatrix = glm::identity<glm::mat4>();
glm::mat4 viewMatrix = glm::identity<glm::mat4>();

float fovy = 45;
float scale = 1.1f;
double cur_x_pos;
double cur_y_pos;
bool isMousePresed = false;
bool isMouse3Presed = false;

#pragma region Window callback

void scroled(GLFWwindow* win, double xoffset, double yoffset)
{
    GWindow2d* window = GWindow2dManger::Instanse()->GetWindow(win);
    auto mat = glm::identity<glm::mat4>(); //glm::translate(viewMatrix, glm::vec3(1.f));
    if (yoffset == 1)
    {   
        window->ScaleObjects(1.05f, 1.05f);
        //viewMatrix  = glm::scale(viewMatrix, glm::vec3(1.05f, 1.05f, 1.f));
    }
    else
    {
        window->ScaleObjects(0.95, 0.95);
        //viewMatrix = glm::scale(viewMatrix, glm::vec3(0.95, 0.95f, 1.f));
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == GLFW_KEY_F5 && action == GLFW_PRESS)
    {
        GWindow2dManger::Instanse()->GetWindow(window)->ResetTransform();
        viewMatrix *= glm::inverse(viewMatrix);
    }
}

void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
    {       
        isMousePresed = true;
        glfwGetCursorPos(window, &cur_x_pos, &cur_y_pos);
        std::cout << "Mouse1 presed\n";
        //std::cout << cur_x_pos << "\t" << cur_x_pos << std::endl;
        //scene.Add(std::make_shared<GPoint2D>(v));
    }

    if (button == GLFW_MOUSE_BUTTON_3 && action == GLFW_PRESS)
    {
        isMouse3Presed = true;
    }

    if (button == GLFW_MOUSE_BUTTON_3 && action == GLFW_RELEASE)
    {
        isMouse3Presed = false;
    }
    if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE)
    {
        isMousePresed = false;
        std::cout << "Mouse1 realesed\n";
    }
}

void cursor_moved(GLFWwindow* win, double xpos, double ypos)
{
    if (isMousePresed && !isMouse3Presed)
    {   
        glm::vec3 v(-xpos / 100, -ypos/100, 0.f);
        std::cout << xpos << "\t" << ypos << std::endl;
        double xoffset = (cur_x_pos - xpos) /-30;
        double yoffset = (cur_y_pos - ypos) /30 ;
        int z = 3;
        viewMatrix = glm::translate(viewMatrix, glm::vec3(xoffset, yoffset, 0.f));
    }
    if (!isMousePresed && isMouse3Presed)
    {
       /* glm::vec3 v(-xpos / 100, -ypos / 100, 0.f);
        std::cout << xpos << "\t" << ypos << std::endl;
        double xoffset = (cur_x_pos - xpos) / -30;
        double yoffset = (cur_y_pos - ypos) / 30;
        viewMatrix = glm::rotate(viewMatrix,15.f, glm::vec3(0.1, 0.1, 0.f));*/
        GWindow2d* window = GWindow2dManger::Instanse()->GetWindow(win);
        window->RotateObjects(15, 0, 0);
    }
}

void on_resize(GLFWwindow* window, int width, int height)
{
    
    //projectionMatrix = glm::ortho(0.f, (float)width, (float)height, 0.f, 0.f, 1.f);
}

#pragma endregion



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

int main()
{
    
    std::string vertexShaderFile = "Shaders\\simple_vertex_shader.glsl";
    std::string fragmentShaderFile = "Shaders\\simple_fragment_shader.glsl";
  
    GWindow2d window(WIDTH, HEIGHT, "Test");
    auto manager = ResourceManager::Instance();
    ShaderProgramPtr shader = manager->CreateShader("test", vertexShaderFile, fragmentShaderFile);
    if (shader == nullptr)
        return -1;

    shader->SetAtribsLocation({ {0,"vertex_pos"s},{1,"vertex_color"s} });

    window.SetShader(shader);
    glfwSetKeyCallback(window.Handle(),key_callback);
    glfwSetCursorPosCallback(window.Handle(), cursor_moved);
    glfwSetMouseButtonCallback(window.Handle(), mouse_callback);
    glfwSetScrollCallback(window.Handle(), scroled);
    glfwSetWindowSizeCallback(window.Handle(), on_resize);
    
    glPointSize(10);
    GPolygon poly(shader);
    poly.AddPoint(200.0f, 200.f, .0f);
    poly.AddPoint(600.0f, 200.f, .0f);
    poly.AddPoint(200.0f, 600.f, .0f);
    poly.AddPoint(600.0f, 600.f, .0f);
    //GPoint point(200., 200., shader);
    //shader->SetMatrix4("projection", projectionMatrix);


    window.AddGraphicElement(gobject_to_ptr(poly));
    glViewport(0, 0, WIDTH, HEIGHT);
  
    while (!window.IsShouldClose())
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1,1,0,1);
        glColor3f(1.0, 0.f, 0.f);
        glPushMatrix();

       
        glBegin(GL_TRIANGLES);
        glVertex2f(0, 0.5);
        glVertex2f(0,0);
        glVertex2f(0.5,0);
        glRotatef(15, 1, 1, 0);
        glEnd();

        glPopMatrix();
        /*shader->SetMatrix4("model", modelMatrix);*/
        //shader->SetMatrix4("view", viewMatrix);   

        //window.Draw();
        window.SwapBuffer();
    }

    return 0;
}