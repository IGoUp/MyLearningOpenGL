#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

int main(int argc, char* arv[])
{
    // 初始化 GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // 创建窗口并将窗口绑定到当前线程的上下文
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // 通过 GLAD 获取 gl 函数地址
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 设定 viewport 的 size
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 定义三角形在正则坐标下的坐标值
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    unsigned int VBO;
    // 生成 1 个 buffer，buffer index 赋值给 VBO
    glGenBuffers(1, &VBO);
    // 指定 VBO 对应的 buffer 类型为 GL_ARRAY_BUFFER
    // 将 VBO 对应的 buffer 绑定到上下文，后续的操作都是基于当前 buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 把的顶点数据复制到 buffer 中
    // GL_STATIC_DRAW 表示数据不会或几乎不会改变
    // 若指定为 GL_DYNAMIC_DRAW 或 GL_STREAM_DRAW，GPU 会把数据放在能够高速写入的内存部分
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 使用字符串硬编码 shader 源码
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    // 创建 Shader 及其对象
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // 将 Shader 源码绑定到 shader 对象
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // 编译 shader
    glCompileShader(vertexShader);

    // 可省略，用于获取 shader 编译失败后的错误信息
    int  success; // GL_TRUE: 1, GL_FALSE: 0
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 循环处理输入并渲染
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 释放资源
    glfwTerminate();

    return 0;
}

