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

            float Radius() const { return radius_; }
            glm::vec3 Center() const { return center_; }
            glm::vec3 Plane() const { return plane_; }

            float Circumference() const { return fabs(2 * glm::pi<float>() * radius_); }

        protected:
            Circle() = default;

        protected:
            float radius_;
            glm::vec3 plane_;
            glm::vec3 center_;
            
        };

        class Arc : public Circle
        {
        public:

            Arc(const Circle& circle)
            {
                radius_ = circle.Radius();
                plane_ = circle.Plane();
                center_ = circle.Center();
                angleMin_ = 0;
                angleMax_ = 2 * glm::pi<float>();
            }

            Arc(const glm::vec3& center, const glm::vec3& plane, float radius, float angMin, float angMax) : Circle(center, plane,radius)
            {
                angleMin_ = angMin;
                angleMax_ = angMax;
            }

            Arc(const glm::vec3& center, float radius, float angMin, float angMax) : Circle(center, xy_plane, radius)
            {
                angleMin_ = angMin;
                angleMax_ = angMax;
            }

            glm::vec3 PointAt(float p) const
            {
                return center_ +  Common::Mathematic::RotateZ(p, radius_);
            }    

            glm::vec3 Begin() const
            {
                return PointAt(angleMax_);
            }

            glm::vec3 Mid() const
            {
                return PointAt(0.5f * (angleMin_ + angleMax_));
            }

            glm::vec3 End() const
            {
                return PointAt(angleMin_);
            }
            float AngleMin() const { return angleMin_; };
            float AngleMax() const { return angleMax_; };
        private:
            float angleMin_;
            float angleMax_;
        };

        class Line
        {
        public:
            Line(const glm::vec3& start, const glm::vec3& end) : to_(end), from_(start){}

            glm::vec3 Start() const { return to_; }
            glm::vec3 End() const { return from_; }

            glm::vec3 Direction() const { return to_ - from_; }

            glm::vec3 PointAt(float t) const
            {
                float s = 1 - t;
                return
                {
                    (from_.x == to_.x) ? from_.x : s * from_.x + t * to_.x,
                     (from_.x == to_.x) ? from_.x : s * from_.y + t * to_.y,
                     (from_.x == to_.x) ? from_.x : s * from_.z + t * to_.z
                };
            }

            bool Intersect(const Line& l2, double x, double y)
            {
               
            }

            GBoundingBox GetBBox() const
            {
                GBoundingBox bbox;
                GBoundingBox::GetPointListBoundingBox(3, 0, 2, 3, glm::value_ptr(from_), bbox, 0, nullptr);
                return bbox;
               
            }
        private:
            glm::vec3 from_;
            glm::vec3 to_;
        };
    }

    namespace Graphic
    {
        class GCircle : public GraphicElementBase
        {
        public:
            DECLARE_G_OBJ(GCircle, GraphicElementBase)

            GCircle(const Common::GeometryPrimitive::Circle& circle) : arc_(circle)
            {

            }

            GCircle(const Common::GeometryPrimitive::Circle& circle, const Resources::ShaderProgramPtr& circleShader) : arc_(circle), selfShader_(circleShader)
            {

            }

            GCircle(const GeometryPrimitive::Arc& arc) : arc_(arc)
            {

            }

        protected:

            virtual GBoundingBox OnGetBBox() const override
            {
                GBoundingBox bbx(arc_.Center() - arc_.Radius(), arc_.Center() + arc_.Radius());
                return bbx;
            }

            virtual void OnDraw() override
            {
                base::OnDraw();
                glEnable(GL_LINE_SMOOTH);
                if (selfShader_ == nullptr)
                {
                    float count = 360.f;
                    float angmin = arc_.AngleMin();
                    float angmax = arc_.AngleMax();
                    glm3Vectors points;
                    for (int i = 0; i < count; i++)
                    {
                        float t = float(i / (count - 1.0));
                        float tt = float((1.0f - t) * angmin + float(t * angmax));
                        points.push_back(arc_.PointAt(tt));
                    }
                    
                    Common::utils::draw_array(points, GL_LINE_STRIP);
                  
                }
                else
                {

                }
                glDisable(GL_LINE_SMOOTH);
            }

        private:
            Common::GeometryPrimitive::Arc arc_;
            Resources::ShaderProgramPtr selfShader_;
        };

        class GLine : public GraphicElementBase
        {   
        public:
            DECLARE_G_OBJ(GLine, GraphicElementBase)

            GLine(const GeometryPrimitive::Line& line) : line_(line)
            {

            }


        protected:

            virtual GBoundingBox OnGetBBox() const override
            {
                return line_.GetBBox();
            }

            virtual void OnDraw() override
            {
                base::OnDraw();

                glm3Vectors vec = { line_.Start() , line_.End() };
                utils::draw_array(vec, GL_LINES);
            }

        private:
            GeometryPrimitive::Line line_;
        };

        class GCube : public GraphicElementBase
        {
        public:
            DECLARE_G_OBJ(GCube, GraphicElementBase)
            GCube(const glm3Vectors& vertexes) : vertexes_(vertexes)
            {

            }

        protected:
            virtual void OnDraw() override
            {
                base::OnDraw();
                utils::draw_array(vertexes_, GL_POINTS);
            }

            virtual GBoundingBox OnGetBBox() const override
            {
                GBoundingBox b;
                GBoundingBox::GetPointListBoundingBox(3, 0, 4, 3, (float*)vertexes_.data(), b, false, nullptr);
                return b;
            }

        private:
           glm3Vectors vertexes_;
           
        };

        class GQuad : public GraphicElementBase
        {
        public:
            DECLARE_G_OBJ(GQuad, GraphicElementBase)

            GQuad(const glm::vec3& p1, const glm::vec3& p2) : bbox_(p1,p2)
            {
                bbox_.GetCorners(corners_);
            }

        protected:
            virtual void OnDraw() override
            {
                base::OnDraw();
                utils::draw_array(corners_, GL_POINTS);
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

#define GL_APP

#ifdef GL_APP

int main()
{
    std::string vertexShaderFile = "Shaders\\simple_vertex_shader_no_color.glsl";
    std::string fragmentShaderFile = "Shaders\\simple_fragment_shader_no_color.glsl";

    GWindow window((GLsizei)WIDTH, (GLsizei)HEIGHT, "Test");
    auto manager = ResourceManager::Instance();
    ShaderProgramPtr shader = manager->CreateShader("test", vertexShaderFile, fragmentShaderFile);
    if (shader == nullptr)
        return -1;
    shader->Use();
    shader->SetAtribsLocation({ {0,"vertex_pos"s} });
  /*  shader->SetAtribsLocation({ {1,"in_color"s} });*/
    window.SetShader(shader);
    

    glm::vec3 p0 = {  0.5, 0.5, 0 };
    glm::vec3 p1 = {  -0.5, 0.5, 0 };
    glm::vec3 p2 = {  -0.5, 0-.5, 0 };
    glm::vec3 p3 = {  0.5, 0-.5, 0 };

    /*glm::vec3 p0 = { 0.0, 0.0, 0 };
    glm::vec3 p1 = { 0.5, 0.5, 0 };
    window.AddGraphicElement(G_MAKE(GQuad)(p0,p1))->SetSize(10);*/
    window.AddGraphicElement(G_MAKE(GPoint)(p0))->SetSize(10);
    window.AddGraphicElement(G_MAKE(GPoint)(p1))->SetSize(10);
    window.AddGraphicElement(G_MAKE(GPoint)(p2))->SetSize(10);
    window.AddGraphicElement(G_MAKE(GPoint)(p3))->SetSize(10);
    window.ZoomAll();

    glEnable(GL_DEPTH_TEST);
    while (!window.IsShouldClose())
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1, 1, 1,1);
        glfwPollEvents();
     

        window.Draw();
        window.SwapBuffer();
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
            rotateAxis.clear();
            glm::vec4 p0t = (projMatrix * viewMatrix )* glm::vec4(curretMousePos,1.0); /*glm::project(curretMousePos, viewMatrix * modelMatrix, projMatrix, viewPort);*/
            glm::vec4 p1t = (projMatrix * viewMatrix) * glm::vec4(newPos, 1.0);//glm::project(newPos, viewMatrix * modelMatrix, projMatrix, viewPort);
           
            glm::vec3 p0 = utils::to_vec3(p0t);
            glm::vec3 p1 = utils::to_vec3(p1t);

            glm::vec3 p = p0;
            glm::vec3 v = p1 - p;

            rotateAxis.emplace_back(p1);
            rotateAxis.emplace_back(p0);
            //test
            glm::vec3 vr = { 0.5,0.5,0 };
            double r = Common::Length2(vr);

            glm::vec3 v1 = p - pc;

            rotateAxis.emplace_back(p);
            rotateAxis.emplace_back(pc);

            auto norm = glm::cross(v1, v);
            
            /*double Lv1 = Common::Length2(v1);
            v1 *= r / Lv1;*/
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
    glViewport(viewPort[0], viewPort[1], viewPort[2], viewPort[3]);
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

    glm::vec3 p0(0.5, 0.5, 0);
    glm::vec3 p1(-0.5, 0.5, 0);
    glm::vec3 p2(-0.5, -0.5, 0);
    glm::vec3 p3(0.5, -0.5, 0);
    glm3Vectors quad;
    quad.push_back(p2);
    quad.push_back(p1);
    quad.push_back(p0);
    quad.push_back(p3);
    //viewMatrix = glm::translate(viewMatrix,glm::vec3(0, -0.5, 0));
    
    //viewMatrix = glm::translate(viewMatrix, glm::vec3(0, 0.5, 0));
    /*auto test = glm::vec4(xaxsis[1],1) * viewMatrix;
    auto test = glm::vec4(y[1],1) * viewMatrix;*/
    //viewMatrix = glm::translate(glm::vec3(0, 0, 0.5));
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
            if(i == 0)
                utils::draw_line(rotateAxis[i], rotateAxis[i + 1], { 1,0,0 });
            else if(i == 2)
                utils::draw_line(rotateAxis[i], rotateAxis[i + 1], { 0,1,0 });
            
        }
        
        glfwSwapBuffers(window);
    }

    return 0;
}
#endif