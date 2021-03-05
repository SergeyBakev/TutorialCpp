#include "stdafx.h"
#include "GWindow.h"


float HEIGHT = 800.f;
float WIDTH = 800.f;

using namespace Common::Mathematic;
using namespace Common::Graphic;
using namespace std::string_literals;
using namespace std::chrono_literals;
using namespace Common::Resources;
using namespace Common;
using namespace glm;

namespace Common
{
    namespace Graphic
    {
       
        class GQuad : public GraphicElementBase
        {
        public:
            DECLARE_G_OBJ(GQuad, GraphicElementBase)

            GQuad(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& p4)
            {
                corners_ = { p1,p2,p3,p4 };
                GBoundingBox::GetPointListBoundingBox(3, 0, 4, 3, (float*)corners_.data(), bbox_, false, nullptr);
                
            }
        protected:
            virtual void OnDraw() override
            {
                base::OnDraw();
                glPointSize(10);
                Graphic::GVertexBuffer v;
                v.Atach(corners_.data(), (GLsizei)(corners_.size() * sizeof(corners_[0])));
                v.Bind();
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
                glEnableVertexAttribArray(0);

                
                glDrawArrays(GL_QUADS, 0, (GLsizei)(corners_.size()));
                v.Unbind();
            }

            virtual GBoundingBox OnGetBBox() const override
            {
                return bbox_;
            }

        private:
            glm3Vectors corners_;
            GBoundingBox bbox_;
        };
    }
}

using namespace Common::GeometryPrimitive;


bool IntersectLine(Line& line1, Line& line2, glm::vec2& pnt)
{
    auto dir1= line1.Direction();
    auto dir2= line2.Direction();
    
    return false;
}

void print(glm::mat4& m);
void print(glm::vec4& vec);
void print(glm::vec3& vec);

float RangedRand(float fMin, float fMax)
{
    float f = (float)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}
//#define GL_APP
#define OPENGL_DRAW
#ifdef GL_APP

void DrawCube(GWindow& window)
{
    //// Top face(y = 1.f)
    {
        glm::vec3 p0(1.f, 1.f, -1.f);
        glm::vec3 p1(-1.f, 1.f, -1.f);
        glm::vec3 p2(-1.f, 1.f, 1.f);
        glm::vec3 p3(1.f, 1.f, 1.f);
        window.AddGraphicElement(G_MAKE(GQuad)(p0, p1, p2, p3))->SetSize(10)
                                                                ->SetColor(RED);


    }
    
    // Bottom face (y = -1.f)
    {
        glm::vec3 p0(1.f, -1.f, 1.f);
        glm::vec3 p1(-1.f, -1.f, 1.f);
        glm::vec3 p2(-1.f, -1.f, -1.f);
        glm::vec3 p3(1.f, -1.f, -1.f);
        window.AddGraphicElement(G_MAKE(GQuad)(p0, p1, p2, p3))->SetSize(10)->SetColor(GREEN);
    }
    //Front face  (z = 1.f)
    {
        glm::vec3 p0(1.f, 1.f, 1.f);
        glm::vec3 p1(-1.f, 1.f, 1.f);
        glm::vec3 p2(-1.f, -1.f, 1.f);
        glm::vec3 p3(1.f, -1.f, 1.f);
        window.AddGraphicElement(G_MAKE(GQuad)(p0, p1, p2, p3))->SetSize(10)->SetColor(BLUE);
    }


    // Back face (z = -1.f)
    {
        glm::vec3 p0(1.f, -1.f, -1.f);
        glm::vec3 p1(-1.f, -1.f, -1.f);
        glm::vec3 p2(-1.f, 1.f, -1.f);
        glm::vec3 p3(1.f, 1.f, -1.f);
        window.AddGraphicElement(G_MAKE(GQuad)(p0, p1, p2, p3))->SetSize(10)->SetColor(BLACK);
    }


    // Left face (x = -1.f)
    {
        glm::vec3 p0(-1.f, 1.f, 1.f);
        glm::vec3 p1(-1.f, 1.f, -1.f);
        glm::vec3 p2(-1.f, -1.f, -1.f);
        glm::vec3 p3(-1.f, -1.f, 1.f);
        window.AddGraphicElement(G_MAKE(GQuad)(p0, p1, p2, p3))->SetSize(10)->SetColor({ 1,0.5,1 });
    }


    //// Right face (x = 1.f)
    {
        glm::vec3 p0(1.f, 1.f, -1.f);
        glm::vec3 p1(1.f, 1.f, 1.f);
        glm::vec3 p2(1.f, -1.f, 1.f);
        glm::vec3 p3(1.f, -1.f, -1.f);
        window.AddGraphicElement(G_MAKE(GQuad)(p0, p1, p2, p3))->SetSize(10)->SetColor({ 0.5,0.5,1 });
    }
}

int main()
{
    std::string vertexShaderFile = "Shaders\\simple_vertex_shader.glsl";
    std::string fragmentShaderFile = "Shaders\\simple_fragment_shader.glsl";

    GWindow window((GLsizei)WIDTH, (GLsizei)HEIGHT, "Test");
    auto manager = ResourceManager::Instance();
    ShaderProgramPtr shader = manager->CreateShader("test", vertexShaderFile, fragmentShaderFile);
    if (shader == nullptr)
        return -1;

    shader->Use();
    shader->SetAtribsLocation({ {0,"vertex_pos"s} });
    shader->SetAtribsLocation({ {1,"in_color"s} });
    shader->SetAtribsLocation({ {2,"lightColor"s} });
    shader->SetAtribsLocation({ {3,"ambientStrength"s} });
    GVertexBuffer l;
    glm::vec3 lightColor = { 1,1,0 };
  
    window.SetShader(shader);
    
    //DrawCube(window);
    {
        glm::vec3 p0(1.f, 1.f, 1.f);
        glm::vec3 p1(-1.f, 1.f, 1.f);
        glm::vec3 p2(-1.f, -1.f, 1.f);
        glm::vec3 p3(1.f, -1.f, 1.f);
        window.AddGraphicElement(G_MAKE(GQuad)(p0, p1, p2, p3))->SetSize(10)->SetColor(BLUE);
    }

    //window.ZoomAll();

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    while (!window.IsShouldClose())
    {
        glClearColor(1, 1, 1, 1);
        glClearDepth(1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        glfwPollEvents();
     

        window.Draw();
        window.SwapBuffer();
    }
  
    return 0;
}

#else
#ifdef OPENGL_DRAW

GLdouble m_ModelMatrix[16];
GLdouble m_ProjMatrix[16];
GLint m_Viewport[4];

glm3Vectors points;
glm3Vectors quad;
glm3Vectors spaceCube;
glm3Vectors spaceEdge;
glm3Vectors quadEdges;
glm3Vectors dragVector2;

glm3Vectors toStartpoint2;
glm3Vectors toEndpoint2;

glm3Vectors normalVector;
glm3Vectors P1P0;

glm3Vectors tickPoint2;
glm3Vectors startTickPoint2;

glm::vec3 curentMousePos;

double& GetSizeSpace()
{
    static double spaceSize;
    return spaceSize;
}
bool OnUpdateSpaceSize();

void SetSpaceSize(double val)
{
    GetSizeSpace() = val;
    OnUpdateSpaceSize();
}

void UpdateSizeSpace()
{

}
bool OnUpdateSpaceSize()
{
    int cx = m_Viewport[2] + m_Viewport[0];
    int cy = m_Viewport[3] + m_Viewport[1];
    if (cx < 15 || cy < 15)
        return false;

    // производим нормировку высоты и ширины
    // для получения квадрата квадратным
    GLdouble rw, rh, rz;
    rz = rw = rh = GetSizeSpace();

    // устанавливаем GL_PROJECTION
    GLdouble fAspect = (GLdouble)cx / (GLdouble)cy;
    if (cx > cy)
        rw *= fAspect;
    else
        rh /= fAspect;

    // устанавливаем двухмерную ортогональную матрицу
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-rw, rw, -rh, rh, -rz, rz);
    glGetDoublev(GL_PROJECTION_MATRIX, m_ProjMatrix);
    glMatrixMode(GL_MODELVIEW);

}
glm::vec3 Unproject(const glm::vec3& pos)
{
    double x, y, z;
    gluUnProject(pos.x, pos.y, pos.z,m_ModelMatrix,m_ProjMatrix,m_Viewport,&x,&y,&z);
    return { x,y,z };
}

glm::vec3 Project(const glm::vec3& pos)
{
    double x, y, z;
    gluProject(pos.x, pos.y, pos.z, m_ModelMatrix, m_ProjMatrix, m_Viewport, &x, &y, &z);
    return { x,y,z };
}


void OnRotate(const glm::vec3& vec);

namespace CallBack
{
    bool isMouseBnt1Presed;
    bool isMouseBnt3Presed;

    void mouse_callback(GLFWwindow* window, int button, int action, int mods)
    {
        if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
        {
            double x, y;
            glfwGetCursorPos(window, &x, &y);
            y = 800 - y;
            auto pos = Unproject({ x,y,0 });
            //points.push_back(pos);
            //std::cout << "GLFW_MOUSE_BUTTON_1 GLFW_PRESSED:\t" << x << "\t" << y << std::endl;
            isMouseBnt1Presed = true;
        }

        if (button == GLFW_MOUSE_BUTTON_3 && action == GLFW_PRESS)
        {
            isMouseBnt3Presed = true;
        }

        if (button == GLFW_MOUSE_BUTTON_3 && action == GLFW_RELEASE)
        {
            isMouseBnt3Presed = false;
        }
        if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE)
        {

            isMouseBnt1Presed = false;
            //std::cout << "GLFW_MOUSE_BUTTON_1 GLFW_RELEASE:\t" <</* cur_x_pos << "\t" << cur_y_pos << */std::endl;
        }

    }

    void cursor_moved(GLFWwindow* window, double xpos, double ypos)
    {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        glm::vec3 newPos(x, 800 - y, 0);

        std::cout << "cursor_moved:" << x << "\t" << y << std::endl;
        if (isMouseBnt1Presed && !isMouseBnt3Presed)
        {
            //points.push_back(Unproject(newPos));
            auto curPos = Unproject(curentMousePos);
            auto translate = Unproject(newPos) - curPos;

            glTranslatef(translate.x, translate.y, translate.z);
            glGetDoublev(GL_MODELVIEW_MATRIX, m_ModelMatrix);
        }

        if (isMouseBnt3Presed && !isMouseBnt1Presed)
        {
            OnRotate(newPos);
            
    
        }

        curentMousePos = newPos;
        
    }
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
        if (key == GLFW_KEY_HOME && action == GLFW_PRESS)
        {
            points.clear();
            toStartpoint2.clear();
            toEndpoint2.clear();
            glLoadIdentity();
            glMatrixMode(GL_PROJECTION);
            glGetDoublev(GL_PROJECTION_MATRIX, m_ProjMatrix);

            glLoadIdentity();
            glMatrixMode(GL_MODELVIEW);
            glGetDoublev(GL_MODELVIEW_MATRIX, m_ModelMatrix);
        }
        if (key == GLFW_KEY_UP && action == GLFW_PRESS)
        {
            glRotatef(5, 1, 0, 0);
            glGetDoublev(GL_MODELVIEW_MATRIX, m_ModelMatrix);
        }

        if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        {
            glRotatef(-5, 1, 0, 0);
            glGetDoublev(GL_MODELVIEW_MATRIX, m_ModelMatrix);
        }

        if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        {
            glRotatef(-5, 0, 1, 0);
            glGetDoublev(GL_MODELVIEW_MATRIX, m_ModelMatrix);
            
        }
        if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        {
            glRotatef(5, 0, 1, 0);
        }
    }
    

    void scroled(GLFWwindow* win, double xoffset, double yoffset)
    {  
        glMatrixMode(GL_PROJECTION);
        if (yoffset == 1)
        {
            glScalef(1.05f, 1.05, 1.f);
        }
        else
        {
            glScalef(0.95f, 0.95, 1.f);
        }

        glGetDoublev(GL_PROJECTION_MATRIX, m_ProjMatrix);
        glMatrixMode(GL_MODELVIEW);
    }
}

void OnRotate(const glm::vec3& vec)
{
    auto p0 = Unproject(curentMousePos);
    auto p1 = Unproject(vec);
    glm::vec3 v = p1 - p0;
    GBoundingBox m_DimModel;
    auto data = (float*)quad.data();
    GBoundingBox::GetPointListBoundingBox(3, 0, quad.size(), 3, data, m_DimModel, 0,nullptr);
    if (!Common::detial::IsValid(v))
        return;
    glm::vec3 pc = m_DimModel.Center();
    float r = glm::length(glm::vec3(m_DimModel.Max() - pc));
    glm::vec3 v1 = (p0 - pc);
    float Lv1 = glm::length(v1);
    v1 *= r / Lv1;
 
    glm::vec3 norm = glm::cross(v1, v);
    glm::vec3 v2 = v1 + v;

    norm = glm::normalize(norm);
    v1 = glm::normalize(v1);
    v2 = glm::normalize(v2);

  
    float cosg = acos(glm::dot(v1, v2));
    float angle = glm::degrees(cosg);
    glTranslatef(pc.x, pc.y, pc.z);
    glRotatef(angle/*0.5*/, norm.x, norm.y, norm.z);
    glTranslatef(-pc.x, -pc.y, -pc.z);

   
}

void Initialize(GLFWwindow*& window)
{
    if (!glfwInit())
        throw std::runtime_error("Could not initialize GLFW");

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    window = glfwCreateWindow(static_cast<int>(800), static_cast<int>(800), "Test", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        throw std::runtime_error("Could not initialize GLFW");
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
        throw std::runtime_error("Could not initialize GLEW");

    m_Viewport[0] = 0;
    m_Viewport[1] = 0;
    m_Viewport[2] = 800;
    m_Viewport[3] = 800;

    glfwSetKeyCallback(window, CallBack::key_callback);
    glfwSetCursorPosCallback(window, CallBack::cursor_moved);
    glfwSetMouseButtonCallback(window, CallBack::mouse_callback);
    glfwSetScrollCallback(window, CallBack::scroled);
    glViewport((GLint)m_Viewport[0], (GLint)m_Viewport[1], (GLint)m_Viewport[2], (GLint)m_Viewport[3]);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -2, 2);
    glGetDoublev(GL_PROJECTION_MATRIX, m_ProjMatrix);

    glMatrixMode(GL_MODELVIEW);
    glGetDoublev(GL_MODELVIEW_MATRIX, m_ModelMatrix);
    
    glGetIntegerv(GL_VIEWPORT, m_Viewport);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
}

void AddCube()
{
    //// Top face(y = 0.5f)
    {
        glm::vec3 p0(0.5f, 0.5f, -0.5f);
        glm::vec3 p1(-0.5f, 0.5f, -0.5f);
        glm::vec3 p2(-0.5f, 0.5f, 0.5f);
        glm::vec3 p3(0.5f, 0.5f, 0.5f);

        quad.push_back(p0);
        quad.push_back(p1);
        quad.push_back(p2);
        quad.push_back(p3);

        quadEdges.push_back(p0);
        quadEdges.push_back(p1);
        quadEdges.push_back(p2);
        quadEdges.push_back(p3);
    }

    // Bottom face (y = -0.5f)
    {
        glm::vec3 p0(0.5f, -0.5f, 0.5f);
        glm::vec3 p1(-0.5f, -0.5f, 0.5f);
        glm::vec3 p2(-0.5f, -0.5f, -0.5f);
        glm::vec3 p3(0.5f, -0.5f, -0.5f);
        quad.push_back(p0);
        quad.push_back(p1);
        quad.push_back(p2);
        quad.push_back(p3);

        quadEdges.push_back(p0);
        quadEdges.push_back(p1);
        quadEdges.push_back(p2);
        quadEdges.push_back(p3);
    }
    //Front face  (z = 0.5f)
    {
        glm::vec3 p0(0.5f, 0.5f, 0.5f);
        glm::vec3 p1(-0.5f, 0.5f, 0.5f);
        glm::vec3 p2(-0.5f, -0.5f, 0.5f);
        glm::vec3 p3(0.5f, -0.5f, 0.5f);
        quad.push_back(p0);
        quad.push_back(p1);
        quad.push_back(p2);
        quad.push_back(p3);

        quadEdges.push_back(p0);
        quadEdges.push_back(p1);
        quadEdges.push_back(p2);
        quadEdges.push_back(p3);
    }


    // Back face (z = -0.5f)
    {
        glm::vec3 p0(0.5f, -0.5f, -0.5f);
        glm::vec3 p1(-0.5f, -0.5f, -0.5f);
        glm::vec3 p2(-0.5f, 0.5f, -0.5f);
        glm::vec3 p3(0.5f, 0.5f, -0.5f);
        quad.push_back(p0);
        quad.push_back(p1);
        quad.push_back(p2);
        quad.push_back(p3);

        quadEdges.push_back(p0);
        quadEdges.push_back(p1);
        quadEdges.push_back(p2);
        quadEdges.push_back(p3);
    }


    // Left face (x = -0.5f)
    {
        glm::vec3 p0(-0.5f, 0.5f, 0.5f);
        glm::vec3 p1(-0.5f, 0.5f, -0.5f);
        glm::vec3 p2(-0.5f, -0.5f, -0.5f);
        glm::vec3 p3(-0.5f, -0.5f, 0.5f);
        quad.push_back(p0);
        quad.push_back(p1);
        quad.push_back(p2);
        quad.push_back(p3);

        quadEdges.push_back(p0);
        quadEdges.push_back(p1);
        quadEdges.push_back(p2);
        quadEdges.push_back(p3);
    }


    //// Right face (x = 0.5f)
    {
        glm::vec3 p0(0.5f, 0.5f, -0.5f);
        glm::vec3 p1(0.5f, 0.5f, 0.5f);
        glm::vec3 p2(0.5f, -0.5f, 0.5f);
        glm::vec3 p3(0.5f, -0.5f, -0.5f);
        quad.push_back(p0);
        quad.push_back(p1);
        quad.push_back(p2);
        quad.push_back(p3);

        quadEdges.push_back(p0);
        quadEdges.push_back(p1);
        quadEdges.push_back(p2);
        quadEdges.push_back(p3);
    }
}

void DrawSpaceCube()
{
    spaceCube.clear();
    spaceEdge.clear();
    //// Top face(y = 1.f)
    {
        glm::vec3 p0(1.f, 1.f, -1.f);
        glm::vec3 p1(-1.f, 1.f, -1.f);
        glm::vec3 p2(-1.f, 1.f, 1.f);
        glm::vec3 p3(1.f, 1.f, 1.f);

        spaceCube.push_back(p0);
        spaceCube.push_back(p1);
        spaceCube.push_back(p2);
        spaceCube.push_back(p3);

        spaceEdge.push_back(p0);
        spaceEdge.push_back(p1);
        spaceEdge.push_back(p2);
        spaceEdge.push_back(p3);

    }

    // Bottom face (y = -1.f)
    {
        glm::vec3 p0(1.f, -1.f, 1.f);
        glm::vec3 p1(-1.f, -1.f, 1.f);
        glm::vec3 p2(-1.f, -1.f, -1.f);
        glm::vec3 p3(1.f, -1.f, -1.f);
        spaceCube.push_back(p0);
        spaceCube.push_back(p1);
        spaceCube.push_back(p2);
        spaceCube.push_back(p3);

        spaceEdge.push_back(p0);
        spaceEdge.push_back(p1);
        spaceEdge.push_back(p2);
        spaceEdge.push_back(p3);
    }
    //Front face  (z = 1.f)
    {
        glm::vec3 p0(1.f, 1.f, 1.f);
        glm::vec3 p1(-1.f, 1.f, 1.f);
        glm::vec3 p2(-1.f, -1.f, 1.f);
        glm::vec3 p3(1.f, -1.f, 1.f);
        spaceCube.push_back(p0);
        spaceCube.push_back(p1);
        spaceCube.push_back(p2);
        spaceCube.push_back(p3);

        spaceEdge.push_back(p0);
        spaceEdge.push_back(p1);
        spaceEdge.push_back(p2);
        spaceEdge.push_back(p3);
    }


    // Back face (z = -1.f)
    {
        glm::vec3 p0(1.f, -1.f, -1.f);
        glm::vec3 p1(-1.f, -1.f, -1.f);
        glm::vec3 p2(-1.f, 1.f, -1.f);
        glm::vec3 p3(1.f, 1.f, -1.f);
        spaceCube.push_back(p0);
        spaceCube.push_back(p1);
        spaceCube.push_back(p2);
        spaceCube.push_back(p3);

        spaceEdge.push_back(p0);
        spaceEdge.push_back(p1);
        spaceEdge.push_back(p2);
        spaceEdge.push_back(p3);
    }


    // Left face (x = -1.f)
    {
        glm::vec3 p0(-1.f, 1.f, 1.f);
        glm::vec3 p1(-1.f, 1.f, -1.f);
        glm::vec3 p2(-1.f, -1.f, -1.f);
        glm::vec3 p3(-1.f, -1.f, 1.f);
        spaceCube.push_back(p0);
        spaceCube.push_back(p1);
        spaceCube.push_back(p2);
        spaceCube.push_back(p3);

        spaceEdge.push_back(p0);
        spaceEdge.push_back(p1);
        spaceEdge.push_back(p2);
        spaceEdge.push_back(p3);

    }


    //// Right face (x = 1.f)
    {
        glm::vec3 p0(1.f, 1.f, -1.f);
        glm::vec3 p1(1.f, 1.f, 1.f);
        glm::vec3 p2(1.f, -1.f, 1.f);
        glm::vec3 p3(1.f, -1.f, -1.f);
        spaceCube.push_back(p0);
        spaceCube.push_back(p1);
        spaceCube.push_back(p2);
        spaceCube.push_back(p3);

    }
}
int main()
{
    GLFWwindow* window = nullptr;
    Initialize(window);

    AddCube();
    GBoundingBox m_DimModel;
    auto data = (float*)quad.data();
    GBoundingBox::GetPointListBoundingBox(3, 0, quad.size(), 3, data, m_DimModel, 0, nullptr);
    SetSpaceSize(m_DimModel.MaximumDistanceTo({ 0,0,0 }));
    OnUpdateSpaceSize();
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1, 1, 1, 1);
        glPointSize(10);     
       
        glBegin(GL_QUADS);
        glColor3f(0.1, 0.5, 1);
        for (auto& p : quad)
            glVertex3f(p.x, p.y, p.z);
        glEnd();
       
        glBegin(GL_POINTS);
        glColor3f(0, 0, 1);
        for (auto& p : points)
            glVertex3f(p.x, p.y, p.z);
        glEnd();
        
        glColor3f(0, 0, 0);
        glLineWidth(5);
        glBegin(GL_LINE_LOOP);
        for (auto& p : quadEdges)
            glVertex3f(p.x, p.y, p.z);
        glEnd();
      
      
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    return 0;
}
#else

glm::mat4 viewMatrix = glm::identity<glm::mat4>();
glm::mat4 modelMatrix = glm::identity<glm::mat4>();
glm::mat4 projMatrix = glm::identity<glm::mat4>();
glm::vec3 curretMousePos;

glm::vec3 pc = { 0,0,0 };

glm::vec4 viewPort;

glm3Vectors rotateAxis;

namespace CallBack
{
    bool isMouseBnt1Presed;
    bool isMouseBnt3Presed;

    void mouse_callback(GLFWwindow* window, int button, int action, int mods)
    {
        if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
        {
            double x, y;
            glfwGetCursorPos(window, &x, &y);
            //std::cout << "GLFW_MOUSE_BUTTON_1 GLFW_PRESSED:\t" << x << "\t" << y << std::endl;
            isMouseBnt1Presed = true;
        }

        if (button == GLFW_MOUSE_BUTTON_3 && action == GLFW_PRESS)
        {
            isMouseBnt3Presed = true;
        }

        if (button == GLFW_MOUSE_BUTTON_3 && action == GLFW_RELEASE)
        {
            isMouseBnt3Presed = false;
        }
        if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE)
        {

            isMouseBnt1Presed = false;
            //std::cout << "GLFW_MOUSE_BUTTON_1 GLFW_RELEASE:\t" <</* cur_x_pos << "\t" << cur_y_pos << */std::endl;
        }

    }

    void cursor_moved(GLFWwindow* window, double xpos, double ypos)
    {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        glm::vec3 newPos(x, y, 0);

        std::cout << "cursor_moved:" << x << "\t" << y << std::endl;
        if (isMouseBnt1Presed && !isMouseBnt3Presed)
        {
           
        }

        if (isMouseBnt3Presed && !isMouseBnt1Presed)
        {

        }

        curretMousePos = newPos;

    }
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
        if (key == GLFW_KEY_HOME && action == GLFW_PRESS)
        {
            viewMatrix = glm::identity<glm::mat4>();
        }
        if (key == GLFW_KEY_UP && action == GLFW_PRESS)
        {
            viewMatrix = glm::rotate(viewMatrix, glm::radians(5.f), glm::vec3(1, 0, 0));
        }

        if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        {
            viewMatrix = glm::rotate(viewMatrix, glm::radians(-5.f), glm::vec3(1, 0, 0));
        }

        if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        {
            viewMatrix = glm::rotate(viewMatrix, glm::radians(-5.f), glm::vec3(0, 1, 0));
        }

        if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        {
            viewMatrix = glm::rotate(viewMatrix, glm::radians(5.f), glm::vec3(0, 1, 0));
        }


    }
}

void Initialize(GLFWwindow*& window)
{
    if (!glfwInit())
        throw std::runtime_error("Could not initialize GLFW");

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    window = glfwCreateWindow(static_cast<int>(800), static_cast<int>(800), "Test", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        throw std::runtime_error("Could not initialize GLFW");
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
        throw std::runtime_error("Could not initialize GLEW");

    viewPort = { 0,0,800,800 };
    glfwSetKeyCallback(window, CallBack::key_callback);
    glfwSetCursorPosCallback(window, CallBack::cursor_moved);
    glfwSetMouseButtonCallback(window, CallBack::mouse_callback);
    glViewport((GLint)viewPort[0], (GLint)viewPort[1], (GLint)viewPort[2], (GLint)viewPort[3]);
}


int main()
{
    GLFWwindow* window = nullptr;
    Initialize(window);
    std::string vertexShaderFile = "Shaders\\simple_vertex_shader.glsl";
    std::string fragmentShaderFile = "Shaders\\simple_fragment_shader.glsl";
    auto manager = ResourceManager::Instance();
    ShaderProgramPtr shader = manager->CreateShader("test", vertexShaderFile, fragmentShaderFile);
    if (shader == nullptr)
        return -1;
    shader->Use();
    shader->SetAtribsLocation({ {0,"vertex_pos"s} });
    shader->SetAtribsLocation({ {1,"in_color"s} });


    shader->SetUniformMatrix4("projection", projMatrix);
    projMatrix = glm::ortho(0.f, 800.f, 800.f, 0.f);


    glm3Vectors xaxsis =
    {
        {0,0,0},
        {0.5,0,0}
    };

    glm3Vectors yaxsis =
    {
        {0,0,0},
        {0,0.5,0}
    };

    glm3Vectors zaxsis =
    {
        {0,0,0},
        {0,0,0.5}
    };

    glEnable(GL_DEPTH_TEST);

  
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1, 1, 1, 1);
        glfwPollEvents();

        shader->SetUniformMatrix4("view", viewMatrix);
        shader->SetUniformMatrix4("model", modelMatrix);


        //utils::draw_array_object(quad, GL_QUADS);
        for (size_t i = 0; i < rotateAxis.size(); i += 2)
        {
            glLineWidth(2);
            if (i == 0)
                utils::draw_line(rotateAxis[i], rotateAxis[i + 1], { 1,0,0 });
            else if (i == 2)
                utils::draw_line(rotateAxis[i], rotateAxis[i + 1], { 0,1,0 });

        }

        glfwSwapBuffers(window);
    }

    return 0;
}
#endif // !OPENGL_DRAW


#endif