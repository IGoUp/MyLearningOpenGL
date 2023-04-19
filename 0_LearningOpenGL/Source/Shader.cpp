#include "Shader.h"


Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    // 1. 从文件路径中获取顶点/片段着色器
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // 保证ifstream对象可以抛出异常
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // 打开文件
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // 读取文件的缓冲内容到数据流中
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // 关闭文件处理器
        vShaderFile.close();
        fShaderFile.close();
        // 转换数据流到 string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    // 2. 编译着色器
    unsigned int vertexShader = createVertexShader(vertexCode);
    unsigned int fragShader = createFragShader(fragmentCode);
    shaderProgram = createShaderProgram(vertexShader, fragShader);

}

void Shader::release()
{
    glDeleteProgram(shaderProgram);
}

void Shader::use()
{
    glUseProgram(shaderProgram);
}

void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (int)value);
}

void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
}

unsigned int Shader::createVertexShader(const std::string& vShaderCodes)
{
    const char* vertexShaderSource = vShaderCodes.c_str();

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

    return vertexShader;
}

unsigned int Shader::createFragShader(const std::string& fShaderCode)
{
    const char* fragShaderSource = fShaderCode.c_str();

    unsigned int fragShader;
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragShaderSource, NULL);
    // 编译 shader
    glCompileShader(fragShader);

    // 可省略，用于获取 shader 编译失败后的错误信息
    int  success; // GL_TRUE: 1, GL_FALSE: 0
    char infoLog[512];
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return fragShader;
}

unsigned int Shader::createShaderProgram(unsigned int vertexShader, unsigned int fragShader)
{
    // 创建 shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    // attach 并 link 2 个 shader
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);

    // 可省略，用于获取 shader program link 失败后的错误信息
    int  success; // GL_TRUE: 1, GL_FALSE: 0
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
    }

    // 删除 shader
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);

    return shaderProgram;
}
