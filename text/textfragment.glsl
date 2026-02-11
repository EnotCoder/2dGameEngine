#version 330 core
in vec2 v_texCoord;
out vec4 FragColor;

uniform sampler2D textTexture;
uniform vec4 textColor;

void main() {
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(textTexture, v_texCoord).r);
    FragColor = textColor * sampled;
}
