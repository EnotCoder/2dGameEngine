#ifndef SPRITE_H
#define SPRITE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Sprite
{
public:
    glm::vec2 position = glm::vec2(0.0);
    glm::vec2 scale = glm::vec2(1.0);
    float angle = 0;
    unsigned int Texture = 0;
    void draw();
    bool flip = false;
    int cutX = 1;
    int cutY = 1;
    bool isCollide = false;
    glm::vec2 uvFrame = glm::vec2(1.0);
    glm::vec2 rectSize = glm::vec2(1.0);
};

#endif