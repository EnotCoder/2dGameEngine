#ifndef TEXT_H
#define TEXT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <map>
#include <string>        // ← Добавил
#include <glm/glm.hpp>

struct Character {
    GLuint TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    GLuint Advance;
};

extern std::map<GLchar, Character> Characters;
extern GLuint textVAO, textVBO;

void SetupTextBuffers();
void InitText(int font_size);
void RenderText(unsigned int &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

#endif
