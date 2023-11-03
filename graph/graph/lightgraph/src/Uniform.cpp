/*
** EPITECH PROJECT, 2023
** graph
** File description:
** Uniform.cpp
*/

#include "Uniform.hpp"

template<>
void lg::Uniform<lg::vec3f>::setValue()
{
    glUniform3f(_location, _value.x, _value.y, _value.z);
}

template<>
void lg::Uniform<lg::vec4f>::setValue()
{
    glUniform4f(_location, _value.x, _value.y, _value.z, _value.w);
}

template<>
void lg::Uniform<glm::mat4>::setValue()
{
    glUniformMatrix4fv(_location, 1, GL_FALSE, &_value[0][0]);
}

template<>
void lg::Uniform<glm::mat3>::setValue()
{
    glUniformMatrix3fv(_location, 1, GL_FALSE, &_value[0][0]);
}

template<>
void lg::Uniform<glm::mat2>::setValue()
{
    glUniformMatrix2fv(_location, 1, GL_FALSE, &_value[0][0]);
}

template<>
void lg::Uniform<lg::vec2f>::setValue()
{
    glUniform2f(_location, _value.x, _value.y);
}

template<>
void lg::Uniform<lg::vec2i>::setValue()
{
    glUniform2i(_location, _value.x, _value.y);
}

template<>
void lg::Uniform<lg::vec2u>::setValue()
{
    glUniform2ui(_location, _value.x, _value.y);
}

template<>
void lg::Uniform<lg::vec3i>::setValue()
{
    glUniform3i(_location, _value.x, _value.y, _value.z);
}

template<>
void lg::Uniform<lg::vec3u>::setValue()
{
    glUniform3ui(_location, _value.x, _value.y, _value.z);
}

template<>
void lg::Uniform<lg::vec4i>::setValue()
{
    glUniform4i(_location, _value.x, _value.y, _value.z, _value.w);
}

template<>
void lg::Uniform<lg::vec4u>::setValue()
{
    glUniform4ui(_location, _value.x, _value.y, _value.z, _value.w);
}

template<>
void lg::Uniform<float>::setValue()
{
    glUniform1f(_location, _value);
}

template<>
void lg::Uniform<int>::setValue()
{
    glUniform1i(_location, _value);
}

template<>
void lg::Uniform<unsigned int>::setValue()
{
    glUniform1ui(_location, _value);
}
