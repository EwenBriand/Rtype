#version 420 core

layout (location = 0) in vec2 pixelCoords;


void main() {
    gl_Position = vec4(pixelCoords, 1.0, 1.0);
}

