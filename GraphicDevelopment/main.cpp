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
    }
}

using namespace Common::GeometryPrimitive;



class GCircle2D : public GraphicElementBase
{
public:
    DECLARE_G_OBJ(GCircle2D, GraphicElementBase)
    GCircle2D(const glm::vec3& center, float radius)
    {
        center_ = center;
        radius_ = radius;
    }
protected:

    virtual GBoundingBox OnGetBBox() const override
    {
        GBoundingBox bbx(center_ - radius_, center_ + radius_);
        return bbx;
    }

    virtual void OnDraw()
    {
        glPointSize(2);
        std::vector<glm::vec3> points; 
        /*=
        {
            {200,200,0}
        };*/
        float angle = 0.0f;
        float steep = 0.3f;
        float x = center_[0];
        float y = center_[1];

        //points.push_back(center_);
        while (angle < 360)
        {
            auto radians = glm::radians(angle);
            points.push_back({ x + radius_ * cos(radians), y + radius_ * sin(radians),0 });
            angle += steep;
        }

        GVertexBuffer v;
        v.Atach(points.data(), (GLsizei)(points.size() * sizeof(glm::vec3)));
        v.Bind();
        glEnable(GL_LINE_SMOOTH);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)(points.size()));
        glDisable(GL_LINE_SMOOTH);
        v.Unbind();
    }
private:
    glm::vec3 center_;
    float radius_;
    
};



void print(glm::mat4& m);
void print(glm::vec4& vec);
void print(glm::vec3& vec);

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
    glm::vec3 center(500, 500, 0);
    glm::vec3 to(400, 400 + HEIGHT / 10, 0);
    float r =DistanseTo(center, to);
  /*  Circle c1(vec3(0.5, 0.5, 0.5), 0.4f);
    Circle c2(vec3(-0.5, -0.5, 0.5), 0.4f);
    Circle c3(vec3(0.5, -0.5, 0.5), 0.4f);
    Circle c4(vec3(-0.5, 0.5, 0.5), 0.4f);*/
    GeometryPrimitive::Arc arc(vec3(0.5, 0.5, 0), 0.4f, 0.f, pi<float>());
    window.AddGraphicElement(G_MAKE(GCircle)(arc));

    
    auto b = arc.Begin();
    auto e = arc.End();

    window.AddGraphicElement(G_MAKE(GPoint)(b))->SetSize(10.f);
    window.AddGraphicElement(G_MAKE(GPoint)(e))->SetSize(10.f);
  
   /* window.AddGraphicElement(G_MAKE(GCircle)(c1));
    window.AddGraphicElement(G_MAKE(GCircle)(c2));
    window.AddGraphicElement(G_MAKE(GCircle)(c3));
    window.AddGraphicElement(G_MAKE(GCircle)(c4));*/
    window.ZoomAll();
   /* G_PTR(GCircle2D) crcl = GCircle2D::MakeGCircle2DPtr(vec3(0,0,0), 0.2f);
    G_PTR(GCircle2D) crcl1 = GCircle2D::MakeGCircle2DPtr(vec3(0.5,0.5,0), 0.2f);

    glm3Vectors pnts;
    glm3Vectors pnts2;
    crcl->GetBBox().GetCorners(pnts);
    crcl1->GetBBox().GetCorners(pnts2);
    window.AddGraphicElement(crcl);
    window.AddGraphicElement(crcl1);
    */
   /* for (auto& pnt : pnts)
    {
        window.AddGraphicElement(GPoint::MakeGPointPtr(pnt));
    }
    
    for (auto& pnt : pnts2)
    {
        window.AddGraphicElement(GPoint::MakeGPointPtr(pnt));
    }*/
    //window.ZoomAll();
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
        window.Draw();
        window.SwapBuffer();
    }

    return 0;
}