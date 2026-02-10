#include "sprite.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "rect/rect.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void Sprite::draw()
{   
    if (isCollide)
    {
        areaCollide.push_back({
            AABB(
                glm::vec2(position.x-scale.x/2, position.y-scale.y/2),
                glm::vec2(position.x+scale.x/2, position.y+scale.y/2)
            )
        });
    }

    glm::vec2 uvMin = glm::vec2(uvFrame.x/cutX-rectSize.x/cutX, uvFrame.y/cutY-rectSize.y/cutY);
    glm::vec2 uvMax = glm::vec2(uvFrame.x/cutX, uvFrame.y/cutY);

    DrawRect(Texture,scale,position,angle,flip,
    uvMin,uvMax);
}