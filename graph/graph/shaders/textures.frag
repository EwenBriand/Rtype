#version 420 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D textureSampler;

void main() {
    FragColor = texture(textureSampler, TexCoord);
}
