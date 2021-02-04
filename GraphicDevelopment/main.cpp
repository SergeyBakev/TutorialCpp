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
void DrawAxis()
{
    GLfloat axsisVertex[] =
    {
      1,0,0,
      -1,0,0,
      0,1,0,
      0,-1,0
    };

    GLfloat axsisDiogonalVertex[] =
    {
      1,1,0,
      -1,-1,0,
      1,-1,0,
      -1,1,0
    };

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GVertexBuffer gvertexes;
    gvertexes.Atach(axsisVertex, sizeof(axsisVertex));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_LINES, 0, 4);

  /*  gvertexes.Atach(axsisDiogonalVertex, sizeof(axsisDiogonalVertex));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_LINES, 0, 4);*/
}
using namespace glm;

glm::vec3 Rotate(const vec3& vec,float angle)
{
    float x = vec[0] * cos(angle) - vec[1] * sin(angle);
    float y = vec[0] * sin(angle) + vec[1] * cos(angle);
    float z = 0;
    return { x,y,z };
}
void DrawCircle(float radius, const vec3& axis)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    //viewMatrix = glm::rotate(viewMatrix, glm::radians(1.f), glm::vec3(0, 0, 1));
    float pntSize = 0;
    glGetFloatv(GL_POINT_SIZE, &pntSize);
    glPointSize(1);
    float angle = 0.0;
    std::vector<vec3> points;
    points.push_back({ 0,radius,0 });
    vec3 start(0, radius, 0);
    for (; angle < 360;)
    {
       
        angle += 0.001;
        points.push_back(Rotate(start, angle));
        /*matrix = rotate(matrix,glm::radians(angle), axis);
        auto pnt = start * matrix;
        points.push_back({ pnt.x,pnt.y,pnt.z });*/
    }
    GVertexBuffer gvertexes;
    gvertexes.Atach(points.data(), points.size() * sizeof(GL_FLOAT));
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_POINTS, 0, points.size());

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

    shader->SetAtribsLocation({ {0,"vertex_pos"s} });

    /*shader = manager->CreateShader("grid", gridvertexShaderFile, gridFragmentShaderFile);
    if (shader == nullptr)
        return -1;*/

    GLfloat vertex[] =
    {
      /* 0.0,0.0,0.0,*/
       0.,0.5,0.0,
       /*0.0,0.0,0.0,
       1.0,1.0,0.0*/
     /* -0.5 /2,0.5/2,0,
      0.5 /2,0.5 /2,0.,
      0.5/2,-0.5/2,0,
      -0.5/2,-0.5/2,0*/
    };

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GVertexBuffer gvertexes;
    gvertexes.Atach(vertex, sizeof(vertex));
    glPointSize(10);
    vec3 point (2, 1, 0);
    vec3 ax(1, 1, 0);
    shader->Use();
    auto viewMatrix = glm::identity<glm::mat4>();
    auto viewMatrix2 = viewMatrix;
    shader->SetMatrix4("model", glm::identity<glm::mat4>());
    shader->SetMatrix4("projection", glm::identity<glm::mat4>());
    
    //print(viewMatrix);
    while (!window.IsShouldClose())
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        shader->Use();
        shader->SetMatrix4("view", viewMatrix2);
        //
        DrawAxis();
        gvertexes.Bind();
        glBindVertexArray(vao);
        //viewMatrix = glm::translate(viewMatrix,glm::vec3(-0.0001, 0, 0));
        
        shader->SetMatrix4("view", viewMatrix);
        DrawCircle(0.5, { 0,0,1 });
      /*  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_POINTS, 0, 1);*/


        gvertexes.Unbind();
        window.SwapBuffer();
    }

    return 0;
}