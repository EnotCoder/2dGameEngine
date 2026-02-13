#include <glad/glad.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "shader.h"

unsigned int CreateOnlyShader(std::string code,unsigned int shaderType)
{
    // Создаем вершинный шейдер
    unsigned int vertexShader = glCreateShader(shaderType);
    const char* vSource = code.c_str();
    glShaderSource(vertexShader, 1, &vSource, NULL);
    glCompileShader(vertexShader);

    // Проверяем компиляцию
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ОШИБКА компиляции шейдера:\n" << infoLog << std::endl;
        return -1;
    }

    return vertexShader;
}

std::string GetShaderCode(const char* path)
{
    //Shaders
    std::string Code;
    std::ifstream ShaderFile;

    ShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        ShaderFile.open(path);
        std::stringstream vShaderStream;
        vShaderStream << ShaderFile.rdbuf();
        Code = vShaderStream.str();
        ShaderFile.close();
    }
    catch (const std::ifstream::failure& e) {
        std::cout << "ОШИБКА: Не удалось прочитать шейдеры: " << e.what() << std::endl;
        glfwTerminate();
        return "";
    }

    return Code;
}

unsigned int CreateShader(const char* vs,const char* fs)
{
    //get code
    std::string vertexCode = GetShaderCode(vs);
    std::string fragmentCode = GetShaderCode(fs);

    //создание шейдеров
    unsigned int vertexShader = CreateOnlyShader(vertexCode,GL_VERTEX_SHADER);
    unsigned int fragmentShader = CreateOnlyShader(fragmentCode,GL_FRAGMENT_SHADER);

    // Создаем шейдерную программу
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ОШИБКА линковки программы:\n" << infoLog << std::endl;
        return -1;
    }

    // Удаляем шейдеры (программа остается)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}