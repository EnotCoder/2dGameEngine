#include "player.h"
#include "rect/rect.h"
#include "animSprite/anim.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void player::move(GLFWwindow* window,float deltaTime)
{
    int currStateSpace = glfwGetKey(window, GLFW_KEY_SPACE);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && prevSpaceState == GLFW_RELEASE) {
        velocityY = jumpForce;
    }

    prevSpaceState = currStateSpace;

    if (velocityY > 0.02f) {
        targetRotation += 3.0;  // Наклон вверх при прыжке
    } else if (velocityY < -0.02f) {
        targetRotation -= 2.0;   // Наклон вниз при падении
    }

    // Плавная интерполяция к цели
    float rotationDiff = targetRotation - rotateBird;
    rotateBird += rotationDiff * rotationSpeed * deltaTime;

    // Ограничение угла
    if (rotateBird > 25.0f) rotateBird = 25.0f;
    if (rotateBird < -25.0f) rotateBird = -25.0f;
    targetRotation = rotateBird;

    float targetY = PlayerPos[1] + velocityY * deltaTime * 60.0f;

    AABB vertBox(glm::vec2(PlayerPos[0]-0.1f, targetY-0.1f),
                glm::vec2(PlayerPos[0]+0.1f, targetY+0.1f));


    if (UpdateCollide(areaCollide,vertBox))
    {
        active = false;
    }

    birdSprite.Texture = CreateTexture("tex/Player/AllBird1.png");
    birdSprite.framesCount = 4;
    birdSprite.frameTime = 0.15;
    birdSprite.cutX = 4;
    birdSprite.cutY = 5;

    //active
    if (active)
    {   
        PlayerPos[1] = targetY;
        velocityY += gravity * deltaTime * 60.0f;
    }else{
        birdSprite.stop = true;
        prevSpaceState = GLFW_PRESS;
    }

    if (PlayerPos.y < -1.0 || PlayerPos.y > 1.0)
    {
        active = false;
    }

    birdSprite.play(deltaTime,glm::vec2(0.2,0.2),glm::vec2(PlayerPos[0],PlayerPos[1]),rotateBird,flip);
}
