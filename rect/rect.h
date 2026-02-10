#ifndef RECT_H  // Fix: was RENDERER_H
#define RECT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
//pon
struct AABB {
    glm::vec2 min;
    glm::vec2 max;
    AABB(glm::vec2 m, glm::vec2 M) : min(m), max(M) {}
};

struct CollidableObject {
    AABB box;  // Consistent name
    bool active = true;
};

extern unsigned int VAO;
extern unsigned int shaderProgram;
extern int modelLoc;
extern int texLoc;
extern int projLoc;
extern std::vector<CollidableObject> collidables;
extern std::vector<CollidableObject> areaCollide;

unsigned int CreateTexture(const char* pathName);
void initUniforms();
void DrawRect(unsigned int texture,
glm::vec2 scale, glm::vec2 position, 
float angle, bool flip,glm::vec2 uvMin, glm::vec2 uvMax);

bool UpdateCollide(std::vector<CollidableObject> area, AABB box);
bool AABBIntersects(const AABB& a, const AABB& b);

#endif
