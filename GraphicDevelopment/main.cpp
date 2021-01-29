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

            glDeleteVertexArrays(1, &vao);
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

        Point2D GetPointCord() const { return pnt_; }

    protected:
        virtual void OnDraw() override
        {
            GPoint::OnDraw();
            DrawPoint();  
        }

        virtual void OnDraw2() override
        {
            GPoint::OnDraw2();

            DrawPoint();
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
            glDrawArrays(GL_POINTS, 0, 1);
        }
    private:
        Point2D pnt_;
    };

    using GPoint2DPtr = std::shared_ptr<GPoint2D>;
   
    class GPolygon : public GraphicElementBase
    {
    public:
        GPolygon() : GraphicElementBase({0.0,0.0,0.0})
        {

        }

        void AddPoint(float x, float y, float z = 1)
        {
            vertexes_.push_back(x);
            vertexes_.push_back(y);
            vertexes_.push_back(z);
        }

    protected:
        virtual void OnDraw() override
        {
            GLuint vertexbuffer;
            glGenBuffers(1, &vertexbuffer);

            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            glBufferData(GL_ARRAY_BUFFER, vertexes_.size() * sizeof(float), vertexes_.data(), GL_STATIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
            glEnableVertexAttribArray(0);

            glDrawArrays(GL_LINE_LOOP, 0, vertexes_.size() / 3);
        }

        virtual void OnDraw2() override
        {

        }
    private:
        std::vector<float> vertexes_;
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


    template <class T>
    std::shared_ptr<T> gobject_to_ptr(T& obj)
    {
        
        return std::make_shared<T>(obj);
    }
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

    void SetMatrix4(std::string_view name, const glm::mat4& matrix)
    {
        glUniformMatrix4fv(glGetUniformLocation(id_, name.data()), 1, GL_FALSE, glm::value_ptr(matrix));
    }

    GLuint GetAtribLocation(std::string_view name) const
    {
        return glGetAttribLocation(id_, name.data());
    }

    void SetAtribLocation(GLuint pos, std::string_view name)
    {
        if(GetAtribLocation(name) != -1)
            glBindAttribLocation(id_, pos, name.data());
    }

    void SetAtribsLocation(std::initializer_list < std::pair<GLuint, std::string>> conventions)
    {
        for (auto& conv : conventions)
            SetAtribLocation(conv.first, conv.second);
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
        const auto& it = shaders_map_.find(shaderName);
        
        return std::end(shaders_map_) != it ? shaders_map_[shaderName] : nullptr;
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
glm::mat4 scaleMatrix = glm::identity<glm::mat4>();
bool isMousePresed = false;

//void DrawGrid()
//{   
//    {
//        glPushMatrix();
//        glColor3f(1.0, 0.0, 0.0);
//       
//        scale = 1.0 - abs(sin(3.14 * angle / 90.0) / sqrt(3.0) / 2);
//        glRotatef(angle += 0.09, 0, 0, 1);
//        glScalef(scale, scale, 1);
//        glBegin(GL_LINES);
//        for (float i = -WIDTH / 2; i <= WIDTH / 2; i += 20.0)
//        {
//            glVertex2f(i, -HEIGHT / 2);
//            glVertex2f(i, HEIGHT / 2);
//        }
//        for (float i = -HEIGHT / 2; i <= HEIGHT / 2; i += 20.0)
//        {
//            glVertex2f(-WIDTH / 2, i);
//            glVertex2f(WIDTH / 2, i);
//        }
//        glEnd();
//        glPopMatrix();
//    }
//}

//GLfloat colors[] = { 1.0f,0.f,0.f };

GLfloat colors2[] = { 0.0f,0.f,0.f };
GLfloat colors[] = { 0.0f,0.f,0.f };

void DrawAxis()
{
    /*glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(10);
    glBegin(GL_LINES);
    glVertex2f(0, -1);
    glVertex2f(0, 1);
    glVertex2f(-1,0);
    glVertex2f(1, 0);
    glEnd();
    glPopMatrix();*/

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vertexbuffer;
    GLuint colors_vbo = 0;
    GLfloat xAxis[]{ 400.0f,.0f,.0f, 400.0f,800.0f,.0f, .0f,400.0f,0.f,800.0f,400.0f,.0f};

    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(xAxis), xAxis, GL_STATIC_DRAW);

    glGenBuffers(1, &colors_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors2), &colors2, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_LINES, 0, 4); // Начиная с вершины 0, всего 3 вершины -> один треугольник
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
glm::mat4 modelMatrix = glm::identity<glm::mat4>();
glm::mat4 viewMatrix = glm::identity<glm::mat4>();
glm::mat4 projectionMatrix = glm::identity<glm::mat4>();
float fovy = 45;
float scale = 1.1f;

void scroled(GLFWwindow* win, double xoffset, double yoffset)
{
    GWindow2d* window = GWindow2dManger::Instanse()->GetWindow(win);

    std::cout << xoffset << "\t" << yoffset << std::endl;
    double zoom = 1.1 * yoffset;
    
    //camera.Scale(zoom);
    //projectionMatrix = glm::perspective(fovy++, 1.f, 0.f, 100.f);
    if (yoffset == 1)
    {
        modelMatrix = modelMatrix * scaleMatrix;
        
    }
    else
    {
        modelMatrix = modelMatrix * glm::inverse(scaleMatrix);
    }
    window->SwapBuffer();
    return;
}

void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
    {
        double cur_x_pos;
        double cur_y_pos;
        isMousePresed = true;
        //glfwGetCursorPos(window, &cur_x_pos, &cur_y_pos);
        //scene.Add(std::make_shared<GPoint2D>(cur_x_pos, cur_y_pos));
    }
    if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE)
    {
        isMousePresed = false;
    }
}

void cursor_moved(GLFWwindow* win, double xpos, double ypos)
{
    if (isMousePresed)
    {
        modelMatrix = glm::translate(modelMatrix, glm::vec3(1.f,1.f,1.f));
    }
}

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


//glm::vec3 ChangeBasis(const glm::vec3& v, const glm::vec3& e1, const glm::vec3& e2, const glm::vec3& e3)
//{   
//    Eigen::Matrix3f matrix;
//    matrix.setZero();
//    size_t offset = sizeof(float) * 3;
//    const float* p1 = glm::value_ptr(e1);
//    memcpy(matrix.data(), p1, offset);
//    const float* p2 = glm::value_ptr(e2);
//    memcpy(matrix.data() + 3, p2, offset);
//    const float* p3 = glm::value_ptr(e3);
//    memcpy(matrix.data() + 2 * 3, p3, offset);
//    if (matrix.determinant() != 0)
//    {
//        Eigen::Vector3f a;
//        a.setZero();
//        const float* p4 = glm::value_ptr(v);
//        memcpy(a.data(), p4, offset);
//        std::cout << a << std::endl;
//        Eigen::Vector3f solve = matrix.colPivHouseholderQr().solve(a);
//        std::cout << solve << std::endl;
//        glm::vec3 ret(0);
//        memcpy(solve.data(), glm::value_ptr(ret), 3);
//        return ret;
//    }
//
//    return{};
//}

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
    scaleMatrix = glm::scale(scaleMatrix, glm::vec3(1.05f, 1.05f, 1.f));
    glfwSetCursorPosCallback(window.Handle(), cursor_moved);
    glfwSetMouseButtonCallback(window.Handle(), mouse_callback);
    glfwSetScrollCallback(window.Handle(), scroled);
    //projectionMatrix = glm::translate(projectionMatrix,glm::vec(-1.f, 1.f, 1.f);
    projectionMatrix *= glm::ortho(-(float)WIDTH / 2.f, (float)WIDTH / 2.f, -(float)WIDTH / 2.f, (float)WIDTH / 2.f, -1.f, 1.f);
    //projectionMatrix = glm::ortho(0.f, (float)WIDTH, 0.f, (float)HEIGHT,0.f,1.f);
    
    
    glPointSize(10);
   
    //modelMatrix = glm::translate(modelMatrix,glm::vec3(1.5f, 0.0f, -7.0f));
    GPolygon poly;
    GPoint2D pnt1(200.0f, 200.f);
    GPoint2D pnt2(600.0f, 200.f);
    GPoint2D pnt3(200.0f, 600.f);
    GPoint2D pnt4(600.0f, 600.f);
    GPoint2D pnt5(400.0f, 400.f);
    /*poly.AddPoint(200.0f, 200.f, .0f);
    poly.AddPoint(600.0f, 200.f, .0f);
    poly.AddPoint(200.0f, 600.f, .0f);
    poly.AddPoint(600.0f, 600.f, .0f);
    scene.Add(gobject_to_ptr(poly));*/
    //scene.Add(gobject_to_ptr(pnt1));
    //scene.Add(gobject_to_ptr(pnt2));
    //scene.Add(gobject_to_ptr(pnt3));
    //scene.Add(gobject_to_ptr(pnt4));
    scene.Add(gobject_to_ptr(pnt5));
    shader->Use();
    shader->SetMatrix4("projection", projectionMatrix);
    glm::vec4 v(200.f, 200.f, 0.f, 1.f);
    //modelMatrix = glm::translate(modelMatrix,glm::vec3(100.f,0.f,0.f));
    auto t = projectionMatrix * modelMatrix * v;
    print(t);
    while (!window.IsShouldClose())
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1,1,0,1);

       
        shader->SetMatrix4("model", modelMatrix);
        shader->SetMatrix4("view", viewMatrix);
        
        //DrawAxis();

        
        scene.Render();
        window.SwapBuffer();
    }

    return 0;
}