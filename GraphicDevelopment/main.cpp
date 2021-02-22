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

//#define GL_APP

#ifdef GL_APP

int main()
{
    std::string vertexShaderFile = "Shaders\\simple_vertex_shader.glsl";
    std::string fragmentShaderFile = "Shaders\\simple_fragment_shader.glsl";

    GWindow window((GLsizei)WIDTH, (GLsizei)HEIGHT, "Test");
    auto manager = ResourceManager::Instance();
    ShaderProgramPtr shader = manager->CreateShader("test", vertexShaderFile, fragmentShaderFile);
    if (shader == nullptr)
        return -1;
    shader->SetAtribsLocation({ {0,"vertex_pos"s} });
    window.SetShader(shader);
    

    glm::vec3 p0 = { -1,-1,0 };
    glm::vec3 p1 = {  1, 1, 0 };
    window.AddGraphicElement(G_MAKE(GQuad)(p0, p1));
    window.ZoomAll();

    while (!window.IsShouldClose())
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1, 1, 1,1);
        glfwPollEvents();
     

        window.Draw();
        window.SwapBuffer();
    }
  
    return 0;
}

#else
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
}
int main()
{
    GLFWwindow* window = nullptr;
    Initialize(window);
    std::string vertexShaderFile = "Shaders\\simple_vertex_shader_not_matrix.glsl";
    std::string fragmentShaderFile = "Shaders\\simple_fragment_shader.glsl";
    auto manager = ResourceManager::Instance();
    ShaderProgramPtr shader = manager->CreateShader("test", vertexShaderFile, fragmentShaderFile);
    if (shader == nullptr)
        return -1;

    shader->SetAtribsLocation({ {0,"vertex_pos"s} });

    glm::mat4 identity = glm::identity<glm::mat4>();
    shader->SetMatrix4("projection", identity);
    shader->SetMatrix4("view", identity);
    shader->SetMatrix4("model", identity);
    
  
    glm3Vectors v2 =
    {
        {-0.5,-0.5,0.5},
        {-0.5,0.5,0.5},
        {0.5,0.5,0.5},
        {0.5,-0.5,0.5},
        {-0.5,0.5,-0.5},
        {-0.5,0.5,-0.5},
        {0.5,0.5,-0.5},
        {0.5,-0.5,-0.5},
    };

    glPointSize(10);
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1, 1, 1, 1);
        glfwPollEvents();
        shader->Use();

        utils::draw_array_object(v2, GL_QUADS);

        glfwSwapBuffers(window);
    }

    return 0;
}
#endif