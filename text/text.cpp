#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <map>
#include <string>

#define GLEW_STATIC
#define FREEGLUT_STATIC
// Для FreeType:
extern "C" {
#include <ft2build.h>
#include FT_FREETYPE_H
}

float textQuadVertices[] = {
    0.0f,  0.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    1.0f,  1.0f,  0.0f,

    0.0f,  0.0f,  0.0f,
    1.0f,  1.0f,  0.0f,
    1.0f,  0.0f,  0.0f
};


std::map<char, Character> Characters;

// Инициализировать шрифт
void InitText(const char* font_path = "font.otf", int font_size = 48) {
    FT_Library ft; //иницилизация freeType
    if (FT_Init_FreeType(&ft)) { //проверка иницилизации
        fprintf(stderr, "ERROR::FREETYPE: Can't init FreeType Library\n");
        return;
    }

    FT_Face face;//переменная которая хранит шрифт
    if (FT_New_Face(ft, font_path, 0, &face)) { //загружаем шрифт и проверяет загрузился ли
        fprintf(stderr, "ERROR::FREETYPE: Failed to load font\n");
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, font_size); // размер шрифта в пикселях

    for (unsigned char c = 32; c < 127; c++) { // Цикл по ASCII символам от пробела (32) до ~ (126)

        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) { // загружаем опр символ
            fprintf(stderr, "ERROR::FREETYPE: Failed to load glyph %c\n", c);
            continue;
        }

        //создание Opengl Texture

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        // Настройка обертки текстуры — не растягивать за края
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // Сглаживание — GL_LINEAR = плавные края при масштабировании
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character ch = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };

        Characters[c] = ch; // добавляем символ
    }

    FT_Done_Face(face); // освобождение памяти freetype
    FT_Done_FreeType(ft);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);// убирает padding между строками
}


void RenderText(const std::string& text, float x, float y, float scale = 1.0f) {
    glUseProgram(textShaderProgram);  // ваш текст‑программ

    glm::mat4 proj = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f); // под размер окна
    glUniformMatrix4fv(glGetUniformLocation(textShaderProgram, "proj"), 1, GL_FALSE, glm::value_ptr(proj));

    vec4 color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glUniform4fv(glGetUniformLocation(textShaderProgram, "textColor"), 1, glm::value_ptr(color));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(VAO); // VAO с текстурой и координатами

    for (char c : text) {
        Character ch = Characters[c];

        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        // обновление вершин (или заранее генерировать матрицу)
        float vertices[6 * 4] = {
            xpos,     ypos + h,  0.0, 0.0,
            xpos,     ypos,      0.0, 1.0,
            xpos + w, ypos,      1.0, 1.0,

            xpos,     ypos + h,  0.0, 0.0,
            xpos + w, ypos,      1.0, 1.0,
            xpos + w, ypos + h,  1.0, 0.0
        };

        glBindTexture(GL_TEXTURE_2D, ch.texture);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch.advance >> 6) * scale; // шагнуть по горизонтали
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
}
