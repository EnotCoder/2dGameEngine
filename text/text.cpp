#include "text.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>  // ← ortho()
#include <glm/gtc/type_ptr.hpp>          // ← value_ptr()

std::map<GLchar, Character> Characters;
GLuint textVAO, textVBO;  // Определение

void SetupTextBuffers() {
    glGenVertexArrays(1, &textVAO);
    glGenBuffers(1, &textVBO);
    
    glBindVertexArray(textVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    
    float vertices[6][4] = {
        { 0.0f, 1.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f },
        { 1.0f, 0.0f, 1.0f, 1.0f },
        { 0.0f, 1.0f, 0.0f, 0.0f },
        { 1.0f, 0.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f, 0.0f }
    };
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void InitText(int font_size) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return;
    }

    FT_Face face;
    if (FT_New_Face(ft, "./font.ttf", 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        FT_Done_FreeType(ft);
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (GLubyte c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "ERROR::FREETYPE: Failed to load Glyph " << c << std::endl;
            continue;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, 
                     face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<GLuint>(face->glyph->advance.x)
        };
        Characters[c] = character;
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void RenderText(unsigned int &s, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) {
    glUseProgram(s);

    glm::mat4 projection = glm::ortho(0.0f, 1200.0f, 0.0f, 800.0f);  // Размер окна!
    glUniformMatrix4fv(glGetUniformLocation(s, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    
    glUniform3f(glGetUniformLocation(s, "textColor"), color.x, color.y, color.z);

    glUniform3f(glGetUniformLocation(s, "textColor"), color.x, color.y, color.z);
    
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(textVAO);  // ✅ textVAO вместо VAO

    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); ++c) {
        Character ch = Characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;

        GLfloat vertices[6][4] = {
            { xpos, ypos + h, 0.0, 0.0 },
            { xpos, ypos,      0.0, 1.0 },
            { xpos + w, ypos,  1.0, 1.0 },
            { xpos, ypos + h, 0.0, 0.0 },
            { xpos + w, ypos,  1.0, 1.0 },
            { xpos + w, ypos + h, 1.0, 0.0 }
        };

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, textVBO);  // ✅ textVBO вместо VBO
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (ch.Advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
