#include "anim.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "rect/rect.h"


void anim::play(float deltaTime, glm::vec2 scale, glm::vec2 position, float angle, bool flip)
{   
    if (!stop)
    {
        frameTimer += deltaTime;
        if (frameTimer >= frameTime)
        {
            frameTimer -= frameTime;
            currentFrame += 1.0;
            if (currentFrame >= framesCount)
            {
                currentFrame = 0.0;
                currentUv.x = currentFrame;
            }else{
                currentUv.x = currentFrame;
            }
        }
    }

    glm::vec2 uvMin = glm::vec2(currentUv.x/cutX-1.0/cutX, currentUv.y/cutY-1.0/cutY);
    glm::vec2 uvMax = glm::vec2(currentUv.x/cutX, currentUv.y/cutY);

    DrawRect(Texture, scale, position, angle, flip,
    uvMin, uvMax);
}