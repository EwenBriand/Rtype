#version 420 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

out vec2 TexCoord;

uniform mat4 transformationMatrix;
uniform mat4 view;

void main()
{
    gl_Position = view * transformationMatrix * vec4(position, 1.0f);
    TexCoord = texCoord;
}

