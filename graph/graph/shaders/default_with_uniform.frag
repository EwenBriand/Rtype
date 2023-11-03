#version 420 core
out vec4 FragColor;

uniform vec4 ourColor;

void main()
{
    vec4 endColor = ourColor.bgra;
    FragColor = endColor;
}
