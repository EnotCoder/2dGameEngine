#ifndef ANIM
#define ANIM

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class anim
{
private:
    float currentFrame = 0.0;
    float frameTimer = 0.0;
public:
    bool stop = false;
    glm::vec2 currentUv = glm::vec2(1.0,1.0);
    unsigned int Texture;
    int framesCount = 1;
    float frameTime = 0.0;
    int cutX;int cutY;
    void play(float deltaTime, glm::vec2 scale, glm::vec2 position, float angle, bool flip);
};

#endif