#ifndef PLAYER_H
#define PLAYER_H

#include "rect/rect.h"
#include "animSprite/anim.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class player
{
private:
    //texture
    unsigned int Playertexture = CreateTexture("tex/Player/AllBird1.png");

    //position
    glm::vec2 PlayerPos = glm::vec2(-0.7,0.5);
    float speed = 0.015;
    
    //sprite
    bool flip = false;

    //rotate
    float rotateBird = 0.0;
    float jumpForce = 0.025f;
    float targetRotation = 0.0f;
    float rotationSpeed = 90.0f;
    float lastVelocityY = 0.0f;

    //fall
    float gravity = -0.001f;
    float velocityY = 0.0f;
    int prevSpaceState = GLFW_RELEASE;
    anim birdSprite;

public:
    bool active = true;
    void move(GLFWwindow* window,float deltaTime);
};

#endif
