#include "stdafx.h"
#include "GL/GCContext.h"
#include "Window/GWindow.h"
#include "Window/Window2dManager.h"
#define HEIGHT 800
#define WIDTH  800
namespace GraphicPrimitice
{
    class GraphicElement
    {
    public:
        virtual ~GraphicElement() 
        {
            
        }

        virtual void Draw() = 0;
        virtual void Draw2() = 0;
    };

    using GraphicElementPtr = std::shared_ptr<GraphicElement>;

    struct RGBColor
    {
        float red;
        float green;
        float blue;
    };

    class GraphicElementBase : public GraphicElement
    {
    public:
        virtual void Draw() override
        {
            GLuint vao;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            GLuint colors_vbo = 0;
            glGenBuffers(1, &colors_vbo);
            glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(color_), &color_, GL_DYNAMIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
            glEnableVertexAttribArray(1);

            OnDraw();

            glBindVertexArray(0);
        }

        virtual void Draw2() override
        {
            glColor3f(color_.red, color_.green, color_.blue);
            OnDraw2();
           
        }
        virtual ~GraphicElementBase()
        {
            std::cout << "~GraphicElement" << std::endl;
        }
    protected:
        GraphicElementBase(RGBColor color) : color_(color) {}

        virtual void OnDraw() = 0;
        virtual void OnDraw2() = 0;

    private:
        

    private:
        RGBColor color_;
    };


    class GPoint : public GraphicElementBase
    {

    protected:
        GPoint(RGBColor color = { 1.0f,0.0f,0.0f }) : GraphicElementBase(color) {}

        virtual void OnDraw() override
        {

        }

        virtual void OnDraw2() override
        {

        }
    };

    struct Point2D
    {
        double x_;
        double y_;
        double z_ = 0;
    };

    class GPoint2D : public GPoint
    {
    public:
        GPoint2D(double x, double y)
        {
            pnt_.x_ = x;
            pnt_.y_ = y;
        }
    protected:
        virtual void OnDraw() override
        {
            GPoint::OnDraw();
            DrawPoint();  
        }

        virtual void OnDraw2() override
        {
            GPoint::OnDraw2();

            glBegin(GL_POINTS);
            glVertex3d(pnt_.x_, pnt_.y_, pnt_.z_);
            glEnd();
        }


    private:
        void DrawPoint()
        {
            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(pnt_), &pnt_, GL_DYNAMIC_DRAW);

            glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(0);
            glPointSize(10);
            glDrawArrays(GL_POINTS, 0, 3);
        }
    private:
        Point2D pnt_;
    };


    class GCGraphicScene
    {
    public:
        void Add(GraphicElementPtr el)
        {
            elements_.push_back(el);
        }

        void Render()
        {
            for (const auto el : elements_)
                el->Draw();
        }

        void Clear()
        {
            elements_.clear();
        }

        bool Empty() const { return elements_.empty(); }
    private:


        std::vector<GraphicElementPtr> elements_;
    };
}

using  namespace GraphicPrimitice;

using namespace std::string_literals;
using namespace std::chrono_literals;
GCGraphicScene scene;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

}

void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
    {
        double cur_x_pos;
        double cur_y_pos;
        glfwGetCursorPos(window, &cur_x_pos, &cur_y_pos);
        scene.Add(std::make_shared<GPoint2D>(cur_x_pos, cur_y_pos));
    }
}

class ILogger
{
public:

};

class ShaderProgram : std::enable_shared_from_this<ShaderProgram>
{
public:
    ~ShaderProgram()
    {
        Destroy();
    }

    ShaderProgram() = delete;
    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    ShaderProgram(ShaderProgram&& other) noexcept
    {
        Destroy();

        id_ = other.id_;
        other.id_ = 0;
    }
    void Destroy()
    {
        isDestoroyed_ = true;
        glDeleteShader(id_);
    }

    void Use() const
    {
        isUsed_ = true;
        glUseProgram(id_);
    }

    GLuint Id() const { return id_; }
    bool IsValid() const { return !IsDestoroyed() && id_ != 0; }
    bool IsDestoroyed() const { return isDestoroyed_; }
    bool IsUsed() const { return isUsed_; }

    ShaderProgram(GLuint progID, GLuint vertexShID, GLuint fragmendShID) : id_(progID), vetexShId_(vertexShID), fragmentShId_(fragmendShID)
    {

    }

private:
   
private:
    GLuint id_ = 0;
    GLuint vetexShId_ = 0;
    GLuint fragmentShId_ = 0;
    mutable bool isUsed_ = false;
    bool isDestoroyed_ = false;
};

using ShaderProgramPtr = std::shared_ptr<ShaderProgram>;

struct OldShaderAttribConvention 
{
    static void Convention(const ShaderProgramPtr& shader, GLuint pos,std::string_view variableName)
    {
        glBindAttribLocation(shader->Id(), pos, variableName.data());
    }

    static void Conventions(const ShaderProgramPtr& shader, std::initializer_list < std::pair<GLuint, std::string>> conventions)
    {
        for (auto& conv : conventions)
            Convention(shader, conv.first, conv.second);
    }
};

std::string LoadFileCode(std::string_view file)
{
    std::stringstream ss;
    std::ifstream fs;
    fs.open(file.data());
    if (!fs.is_open())
        return {};

    ss << fs.rdbuf();
    return ss.str();

}

class ResourceManager
{
public:
    using ShaderProgramMap = std::map<std::string, ShaderProgramPtr>;
 
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;

    static ResourceManager* Instance()
    {
        static ResourceManager manager;
        return &manager;
    }

    void InjectLogger(ILogger* log)
    {

    }

    static ShaderProgramPtr SCreateShader(const std::string& shaderName, const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
    {
        return Instance()->CreateShader(shaderName, vertexShaderFilePath, fragmentShaderFilePath);
    }

    static void SDestory()
    {
        Instance()->Destory();
    }

    void Destory()
    {
        for (auto& it : shaders_map_)
            it.second->Destroy();

        shaders_map_.clear();
    }

    ShaderProgramPtr CreateShader(const std::string& shaderName, const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
    {
        ShaderLoader loader(vertexShaderFilePath, fragmentShaderFilePath);
        if (loader.HasError())
        {
            //log write error
            return nullptr;
        }
        shaders_map_.insert({ shaderName,loader.GetShader() });
        return loader.GetShader();
    }

    ShaderProgramPtr GetShader(const std::string& shaderName)
    {
        return shaders_map_.contains(shaderName) ? shaders_map_[shaderName] : nullptr;
    }
    
private:

    class ShaderLoader
    {
    public:
        ShaderLoader() = default;
        ShaderLoader(const ShaderLoader&) = delete;
        ShaderLoader(ShaderLoader&&) = delete;
        ShaderLoader& operator=(const ShaderLoader&) = delete;
        ShaderLoader& operator=(ShaderLoader&&) = delete;

        ShaderLoader(const std::string& vertexShaderFile, const std::string& fragmentFileShader)
        {
            Load(vertexShaderFile, fragmentFileShader);
        }

        bool Load(const std::string& vertexShaderFile, const std::string& fragmentFileShader)
        {
            auto vrtxSHCode = LoadFileCode(vertexShaderFile);
            auto frgmntSHCode = LoadFileCode(fragmentFileShader);
            GLuint vrtxId;
            GLuint frgmntId;
            if (!CreateAndCompileShader(vrtxSHCode, GL_VERTEX_SHADER, vrtxId))
                return false;

            if (!CreateAndCompileShader(frgmntSHCode, GL_FRAGMENT_SHADER, frgmntId))
            {
                glDeleteShader(vrtxId);
                return false;
            }

            GLuint prgrmId = glCreateProgram();
            if (!prgrmId)
                return false;

            glAttachShader(prgrmId, vrtxId);
            glAttachShader(prgrmId, frgmntId);
            glLinkProgram(prgrmId);
            if (!CheckLinkError(prgrmId))
            {
                glDeleteProgram(prgrmId);
                return false;
            }

            shader_ = ShaderProgramPtr(new ShaderProgram(prgrmId, vrtxId, frgmntId));
            return true;
        }

        static ShaderProgramPtr LoadShader(const std::string& vertexShaderFile, const std::string& fragmentFileShader)
        {
            ShaderLoader loader(vertexShaderFile, fragmentFileShader);
            return loader.GetShader();
        }


        ~ShaderLoader()
        {
            if (!IsAtached())
                glDeleteProgram(shader_->Id());
        }

        ShaderProgramPtr GetShader() const { isDetached_ = true; return shader_; }

        bool HasError() const { return IsLoaded() && IsCompiled() && IsLinked(); }
        bool IsCompiled() const { return isCompiled_; }
        bool IsLinked() const { return isCompiled_; }
        bool IsLoaded() const { return isLoaded_; }
        bool IsAtached() const { return isDetached_; }

    private:

        bool CreateAndCompileShader(const std::string& shaderCode, const GLenum shaderType, GLuint& id)
        {
            if (shaderCode.empty())
            {
                isLoaded_ = false;
                return false;
            }

            id = glCreateShader(shaderType);
            if (id == 0)
                return false;

            const char* code = shaderCode.c_str();
            glShaderSource(id, 1, &code, nullptr);
            glCompileShader(id);
            return CheckComplieErrors(id);
        }

        bool CheckComplieErrors(GLuint shaderId) const
        {
            GLint status_compile;
            glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status_compile);
            if (!status_compile)
            {
                isCompiled_ = false;
                GLchar log[1024];
                GLsizei msg_size = 0;
                glGetShaderInfoLog(shaderId, sizeof(log), &msg_size, log);
                std::string errors;
                errors.assign(log, log + msg_size);
                std::cerr << "ERROR::SHADER: Compile time error:\n" << std::endl;
                std::cerr << errors << std::endl;
                return false;
            }
            isCompiled_ = true;
            return true;
        }

        bool CheckLinkError(GLuint programId) const
        {
            GLint link_status;
            glGetProgramiv(programId, GL_LINK_STATUS, &link_status);
            if (!link_status)
            {
                isCompiled_ = false;
                GLchar log[1024];
                GLsizei msg_size = 0;
                glGetShaderInfoLog(programId, sizeof(log), &msg_size, log);
                std::string errors;
                errors.assign(log, log + msg_size);
                std::cerr << "ERROR::SHADER: Link time error:\n" << std::endl;
                std::cerr << errors << std::endl;
                return false;
            }

            isLinked_ = true;
            return true;
        }

    private:
        ShaderProgramPtr shader_;
        mutable bool isCompiled_ = false;
        mutable bool isLinked_ = false;
        mutable bool isLoaded_ = false;
        mutable bool isDetached_ = false;
    };

private:
    ResourceManager() = default;
    ShaderProgramMap shaders_map_;

};
float angle = 0.0f;
float scale = 0.0f;

void DrawGrid()
{   
    {
        glPushMatrix();
        glColor3f(1.0, 0.0, 0.0);
       
        scale = 1.0 - abs(sin(3.14 * angle / 90.0) / sqrt(3.0) / 2);
        glRotatef(angle += 0.09, 0, 0, 1);
        glScalef(scale, scale, 1);
        glBegin(GL_LINES);
        for (float i = -WIDTH / 2; i <= WIDTH / 2; i += 20.0)
        {
            glVertex2f(i, -HEIGHT / 2);
            glVertex2f(i, HEIGHT / 2);
        }
        for (float i = -HEIGHT / 2; i <= HEIGHT / 2; i += 20.0)
        {
            glVertex2f(-WIDTH / 2, i);
            glVertex2f(WIDTH / 2, i);
        }
        glEnd();
        glPopMatrix();
    }
}

void DrawAxis()
{
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(10);
    glBegin(GL_LINES);
    glVertex2f(0, -1);
    glVertex2f(0, 1);
    glVertex2f(-1,0);
    glVertex2f(1, 0);
    glEnd();
    glPopMatrix();
}

void cursor_moved(GLFWwindow* win, double xpos,  double ypos)
{
    //std::cout << xpos << "\t" << ypos << std::endl;
}


class Camera2D
{
public:
    Camera2D(const glm::vec3& pos, float fov, float aspect, float zNear, float zFat)
    {
        prespective_ = glm::perspective(fov, aspect, zNear, zFat);
        position_ = pos;
        forward_ = glm::vec3(0, 0, 1);
        up_ = glm::vec3(0, 1, 0);
    }


    glm::mat4 GetViewProjection() const
    {
        return prespective_ * glm::lookAt(position_, position_ + forward_, up_);
    }

    void Scale(double offset)
    {
       
    }

private:
    glm::mat4 prespective_;
    glm::vec3 position_;
    glm::vec3 forward_;
    glm::vec3 up_;

};

static Camera2D camera{ glm::vec3(0,0,-3),70.0f,(float)WIDTH / (float)HEIGHT,0.01f,1000.f };


void scroled(GLFWwindow* win, double xoffset, double yoffset)
{
    GWindow2d* window = GWindow2dManger::Instanse()->GetWindow(win);

    std::cout << xoffset << "\t" << yoffset << std::endl;
    double zoom = 1 * yoffset;
    camera.Scale(zoom);
    window->SwapBuffer();
    return;
}


int main()
{
    std::string vertexShaderFile = "Shaders\\simple_vertex_shader.txt";
    std::string fragmentShaderFile = "Shaders\\simple_fragment_shader.txt";
    GWindow2d window(WIDTH, HEIGHT, "Test");

    auto manager = ResourceManager::Instance();
    ShaderProgramPtr shader = manager->CreateShader("test", vertexShaderFile, fragmentShaderFile);
    if (shader == nullptr)
        return -1;

    OldShaderAttribConvention::Conventions(shader, { {0,"vertex_pos"s},{1,"vertex_color"s} });
   
    glfwSetCursorPosCallback(window.Handle(), cursor_moved);
    glfwSetMouseButtonCallback(window.Handle(), mouse_callback);
    glfwSetScrollCallback(window.Handle(), scroled);

    scene.Add(std::make_shared<GPoint2D>(-0.5, 0.5));
    scene.Add(std::make_shared<GPoint2D>(0.5, 0.5));
    scene.Add(std::make_shared<GPoint2D>(-0.5, -0.5));
    scene.Add(std::make_shared<GPoint2D>(0.5, -0.5));
    GLuint projectionID = glGetUniformLocation(shader->Id(), "projection");
    GLuint viewID = glGetUniformLocation(shader->Id(), "view");
   
    glm::mat4 view = glm::mat4(1.0f);
    while (!window.IsShouldClose())
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto m = camera.GetViewProjection();
        glUniformMatrix4fv(projectionID, 1, GL_FALSE, &m[0][0]);
        glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);
        shader->Use();
        scene.Render();
        window.SwapBuffer();

        //glfwSwapBuffers(window);   
    }

    return 0;
}