#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>

unsigned int CreateShader();
std::string GetShaderCode(const char* path);
unsigned int CreateOnlyShader(std::string code,unsigned int shaderType);

#endif