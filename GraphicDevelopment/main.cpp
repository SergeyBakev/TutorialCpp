#include "stdafx.h"
#include "Window/GWindow.h"
#include "Window/Window2dManager.h"


float HEIGHT = 800.f;
float WIDTH = 800.f;

using namespace Common::Mathematic;
using namespace Common::Graphic;
using namespace std::string_literals;
using namespace std::chrono_literals;
using namespace Common::Resources;
using namespace glm;

namespace Common
{
    namespace GeometryPrimitive
    {
        static const glm::vec3 xy_plane = glm::vec3(1, 1, 0);
        static const glm::vec3 yz_plane = glm::vec3(0, 1, 1);
        static const glm::vec3 zx_plane = glm::vec3(1, 0, 1);

        class Circle
        {
        public:
            Circle(const glm::vec3& center, const glm::vec3& plane, float radius) :         
                center_(center), plane_(plane), radius_(radius)
            {

            }

            Circle(const glm::vec3& center, float radius) : Circle(center,xy_plane,radius)
            {

            }

            Circle(float center_x, float center_y, float center_z,
                   float x_plane, float y_plane, float z_plane,
                   float radius) : Circle({center_x,center_y,center_z},{x_plane,y_plane,z_plane}, radius)
            {

            }

            Circle(float center_x, float center_y,
                float x_plane, float y_plane,
                float radius) : Circle({ center_x,center_y,0 }, { x_plane,y_plane,0 }, radius)
            {

            }


        private:
            float radius_;
            glm::vec3 plane_;
            glm::vec3 center_;
            
        };

        class Arc : public Circle
        {
            
        };
    }
}

using namespace Common::GeometryPrimitive;



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
        glPointSize(2);
        std::vector<glm::vec3> points; /*=
        {
            {200,200,0}
        };*/
        float angle = 0.0f;
        float steep = 0.01f;
        float x = center_[0];
        float y = center_[1];

        points.push_back(center_);
        while (angle < 360)
        {
            auto radians = glm::radians(angle);
            points.push_back({ x + radius_ * cos(radians), y + radius_ * sin(radians),0 });
            angle += steep;
        }

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


void UpdateShaderMatrix(const ShaderProgramPtr& shader)
{
    shader->SetMatrix4("model", modelMatrix);
    shader->SetMatrix4("view", viewMatrix);

}

void InitializeOpenGL(const ShaderProgramPtr& shader)
{
    //glViewport(0, 0, (GLsizei)WIDTH, (GLsizei)HEIGHT);
    //projectionMatrix = ortho(0.f, WIDTH, HEIGHT, 0.f, 0.f, 1.f);
    shader->SetMatrix4("projection", projectionMatrix);
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
   /* GLfloat vertex[] =
    {
       0.0,0.5,0.0,
    };
    GLuint vao;
    glGenVertexArrays(1, &vao);
     GVertexBuffer gvertexes;
    gvertexes.Atach(vertex, sizeof(vertex));*/
    projectionMatrix = glm::ortho(0.f, (float)WIDTH, (float)HEIGHT, 0.f, 0.f, 1.f);
    glm::vec4 center2(400, 400, 0, 1);
    auto vec2 = projectionMatrix * center2;
    print(vec2);
    std::cout << std::endl;
 
    projectionMatrix = glm::inverse(projectionMatrix);
    glm::vec4 center3(0.5, 0.5, 0, 1);
    vec2 = projectionMatrix * center3;
    print(vec2);
    glPointSize(10);

    InitializeOpenGL(shader);
    UpdateShaderMatrix(shader);
    glm::vec3 center(500, 500, 0);
    glm::vec3 to(400, 400 + HEIGHT / 10, 0);
    float r =DistanseTo(center, to);
   /* GCircle2D d(center, r);
    window.AddGraphicElement(gobject_to_ptr(d))*/;
    GPoint pnt(400, 400);
    //window.AddGraphicElement(gobject_to_ptr(pnt)) ;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glMatrixMode(GL_MODELVIEW);
    while (!window.IsShouldClose())
    {
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
        glClearColor(1, 1, 1,1);
        glfwPollEvents();
       
      
        /*glBindVertexArray(vao);
        gvertexes.Bind();
        glBindVertexArray(vao); 
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_POINTS, 0, 1);
        glBindVertexArray(0);
        gvertexes.Unbind();*/
       // window.Draw();
        window.SwapBuffer();
    }

    return 0;
}