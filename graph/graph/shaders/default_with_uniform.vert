#version 420 core
layout (location = 0) in vec3 aPos;

uniform float u_time;

void main()
{
    // rotate by sin(time)
    float s = sin(u_time);
    float c = cos(u_time);
    mat4 rot = mat4(
        c, 0.0, s, 0.0,
        0.0, 1.0, 0.0, 0.0,
        -s, 0.0, c, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
    gl_Position = rot * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    // gl_Position = vec4(aPos.x, 1, 0, 1.0);
}
