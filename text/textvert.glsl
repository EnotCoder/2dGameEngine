#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

out vec2 v_texCoord;

uniform mat4 proj;
uniform vec2 offset;

void main() {
    vec3 pos = position;
    pos.x *= 10.0;      // масштаб под размер символа
    pos.y *= 10.0;
    pos.xy += offset;
    gl_Position = proj * vec4(pos, 1.0);
    v_texCoord = texCoord;
}
