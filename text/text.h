#ifdef TEXT_H
#define TEXT_H

extern float textQuadVertices[];

extern std::map<char, Character> Characters;

struct Character {
    unsigned int texture; // ID текстуры глифа
    glm::ivec2   size;    // ширина/высота
    glm::ivec2   bearing; // позиционный сдвиг
    unsigned int advance; // горизонтальный шаг до следующего символа
};

void InitText(const char* font_path = "font.otf", int font_size = 48);
void RenderText(const std::string& text, float x, float y, float scale = 1.0f);

#endif