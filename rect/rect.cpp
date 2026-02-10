#include "rect.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

unsigned int VAO;
unsigned int shaderProgram;
int modelLoc = -1;
int texLoc = -1;
int projLoc = -1;
std::vector<CollidableObject> areaCollide;

unsigned int CreateTexture(const char* pathName)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(pathName, &width, &height, &nrChannels, 0);
if (data) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    } else {
        std::cout << "Текстура не загрузилась!" << std::endl;
    }

    return texture;
}

void initUniforms() {
    modelLoc = glGetUniformLocation(shaderProgram, "model");
    texLoc = glGetUniformLocation(shaderProgram, "ourTexture");
    projLoc = glGetUniformLocation(shaderProgram, "projection");
}

void DrawRect(unsigned int texture,
glm::vec2 scale, glm::vec2 position, float angle, bool flip,glm::vec2 uvMin, glm::vec2 uvMax)
{   
    if (modelLoc == -1) initUniforms();
    glUniform1i(texLoc, 0);

    glm::mat4 model = glm::mat4(1.0f);  // Единичная матрица
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f)); // вот тут поворот
    model = glm::scale(model,glm::vec3(scale,0.0f));

    model = glm::rotate(model, glm::radians(flip ? 180.0f: 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //вот тут разворот 

    float vertices[] = {
        // позиции          // UV координаты
        -0.5f,  0.5f, 0.0f,  uvMin.x, uvMin.y,  // левый верхний
         0.5f,  0.5f, 0.0f,  uvMax.x, uvMin.y,  // правый верхний
        -0.5f, -0.5f, 0.0f,  uvMin.x, uvMax.y,  // левый нижний
         0.5f, -0.5f, 0.0f,  uvMax.x, uvMax.y   // правый нижний
    };
    

    unsigned int VBO_temp;
    glGenBuffers(1, &VBO_temp);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_temp);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    
    
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(texLoc, 0); 

    glUseProgram(shaderProgram);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    int width, height;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
    float aspect = (float)width / height;
    glm::mat4 projection = glm::ortho(-aspect, aspect, -1.0f, 1.0f);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
}


bool AABBIntersects(const AABB& a, const AABB& b) {
    return (a.max.x >= b.min.x && a.min.x <= b.max.x &&
            a.max.y >= b.min.y && a.min.y <= b.max.y);
}

bool UpdateCollide(std::vector<CollidableObject> area, AABB box)
{
    for (const auto& obj : area)
    {
        if (AABBIntersects(box, obj.box))
        {   
            return true;
            break;
        }

    }

    return false;
}