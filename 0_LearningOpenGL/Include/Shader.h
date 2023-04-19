#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    unsigned int shaderProgram;

    Shader(const char* vertexPath, const char* fragmentPath);
    void release();
    void use();
    // uniform
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
private:
    unsigned int createVertexShader(const std::string& vShaderCodes);
    unsigned int createFragShader(const std::string& fShaderCode);
    unsigned int createShaderProgram(unsigned int vertexShader, unsigned int fragShader);
};
