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
#include "Mathematic/GMathematic.h"
#include "GL/GVertexBufferObject.h"
#include "GL/GIndexArrayObject.h"
#include "GL/Graphic.h"

float HEIGHT = 800.f;
float WIDTH = 800.f;

using namespace Common::Mathematic;
using namespace Common::Graphic;
using namespace Common::Graphic::detail;
using namespace std::string_literals;
using namespace std::chrono_literals;
using namespace Common::Resources;
using namespace glm;


class GCircle2D : public GraphicElementBase
{
public:

    GCircle2D(const glm::vec3& center, float radius) : GraphicElementBase({ 1,1,1 })
    {
        center_ = center;
        radius_ = radius;
    }
protected:
    virtual void OnDraw()
    {
        std::vector<glm::vec3> points; /*=
        {
            {200,200,0}
        };*/
        float angle = 0.0f;
        float steep = 0.01f;
        float x = 400 + 15;
        float y = 400 + 15;

        points.push_back({ 400,400,0 });
        while (angle < 360)
        {
            auto radians = glm::radians(angle);
            points.push_back({ cos(radians),cos(radians),0 });
            angle += steep;
        }
        glPointSize(10);
        GVertexBuffer v;
        v.Atach(points.data(), points.size() * sizeof(glm::vec3));
        v.Bind();

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_POINTS, 0, (GLsizei)(points.size()));

        v.Unbind();
    }
private:
    glm::vec3 center_;
    float radius_;
    
};

GLfloat colors[] = { 0.0f,0.f,0.f };


void print(glm::mat4& m);
void print(glm::vec4& vec);
void print(glm::vec3& vec);


glm::mat4 scaleMatrix = glm::identity<glm::mat4>();
glm::mat4 modelMatrix = glm::identity<glm::mat4>();
glm::mat4 viewMatrix = glm::identity<glm::mat4>();
glm::mat4 projectionMatrix = glm::identity<glm::mat4>();

double x_pos;
double y_pos;
double x_pos2;
double y_pos2;
bool isMousePresed = false;
bool isMouse3Presed = false;

#pragma region Window callback

void mouse_callback(GWindow2d* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
    {
        glfwGetCursorPos(window->Handle(), &x_pos, &y_pos);
        std::cout << "GLFW_MOUSE_BUTTON_1 GLFW_PRESSED:\t" << x_pos << "\t" << y_pos << std::endl;
      
       
       
        isMousePresed = true;
    }

    if (button == GLFW_MOUSE_BUTTON_3 && action == GLFW_PRESS)
    {

    }

    if (button == GLFW_MOUSE_BUTTON_3 && action == GLFW_RELEASE)
    {

    }
    if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE)
    {
      
        isMousePresed = false;
        std::cout << "GLFW_MOUSE_BUTTON_1 GLFW_RELEASE:\t" <</* cur_x_pos << "\t" << cur_y_pos << */std::endl;
    }
}

void mouse_moved(GWindow2d* window, double xpos, double ypos)
{
    if (isMousePresed && !isMouse3Presed)
    {
        glfwGetCursorPos(window->Handle(), &x_pos2, &y_pos2);
        double xoff = x_pos2 - x_pos;
        double yoff = y_pos2 - y_pos;
        window->Move(xoff / 25, yoff / 25);

        std::cout << "GLFW_MOUSE_BUTTON_1 MOVED_PRESED:\t" << x_pos2 << "\t" << y_pos2 << std::endl;
       
        /*v  = v * window->GetProjectionMatrix();
        print(v);
        window->Move(glm::vec3(v[0], v[1], v[2]));
    */
      
    }

    if (!isMousePresed && isMouse3Presed)
    {
        
    }
}

#pragma endregion

void UpdateShaderMatrix(const ShaderProgramPtr& shader)
{
    
    auto window = GWindow2dManger::Instanse()->GetWindowByName("Test");
    shader->SetMatrix4("model", window->GetModelMatrix());
    shader->SetMatrix4("view", window->GetViewMatrix());
    shader->SetMatrix4("projection", window->GetProjectionMatrix());
}

void InitializeOpenGL(const ShaderProgramPtr& shader)
{
    //glViewport(0, 0, (GLsizei)WIDTH, (GLsizei)HEIGHT);
    //projectionMatrix = ortho(0.f, WIDTH, HEIGHT, 0.f, 0.f, 1.f);
    //shader->SetMatrix4("projection", projectionMatrix);
}

int main()
{
    std::string vertexShaderFile = "Shaders\\simple_vertex_shader.glsl";
    std::string fragmentShaderFile = "Shaders\\simple_fragment_shader.glsl";

    GWindow2d window((GLsizei)WIDTH, (GLsizei)HEIGHT, "Test");
    auto manager = ResourceManager::Instance();
    ShaderProgramPtr shader = manager->CreateShader("test", vertexShaderFile, fragmentShaderFile);
    if (shader == nullptr)
        return -1;

    
    shader->SetAtribsLocation({ {0,"vertex_pos"s} });
    
    window.SetShader(shader);
    window.SetMouseCallBack(mouse_callback);
    window.SetMouseMoveCallBack(mouse_moved);

    std::vector<glm::vec3> vertexes =
    {
        {200,200,0.0}
    };

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glPointSize(10); 

    InitializeOpenGL(shader);
    UpdateShaderMatrix(shader);
    //GCircle2D d({ 0,0,0 }, 0.2);
   // window.AddGraphicElement(gobject_to_ptr(d));
    float angle = 0;
    float step = 1;
    float raduis = 20;
    while (!window.IsShouldClose())
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1, 1, 1,1);
        glfwPollEvents();

      
        glm::vec3 xy(200, 200,0);
        window.AddGraphicElement(std::make_shared<GPoint>(xy));
        while (angle < 360)
        {
            vec3 vec = xy;
            vec = RotateZ(vec, angle, raduis);
            vec[0] += xy[0];
            vec[1] += xy[1];
            window.AddGraphicElement(std::make_shared<GPoint>(vec));
            angle += step;
        }

        window.Draw();
        window.SwapBuffer();
    }

    return 0;
}