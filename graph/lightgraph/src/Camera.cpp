/*
** EPITECH PROJECT, 2023
** graph
** File description:
** Camera.cpp
*/

#include "ShaderBuildSystem.hpp"
#include "../include/Camera.hpp"
#include "glm/gtx/string_cast.hpp"

namespace lg {
    bool Camera::ShaderIsSetup = false;

    Camera::Camera()
    {
        if (!ShaderIsSetup) {
            ShaderIsSetup = true;
            auto &spriteShaderBuilder = ShaderBuildSystem::Get()["spriteShader"];
            auto &vertex = spriteShaderBuilder.GetVertexShader();

            vertex.Add(UniformCommon("perspective", "mat4"));
            vertex.AddToMain(
                "gl_Position = perspective * gl_Position;\n", 1000
            );

            vertex.Add(UniformCommon("view", "mat4"));
            vertex.AddToMain(
                "gl_Position = view * gl_Position;\n", 1001
            );

        }

    }

    glm::vec3 Camera::GetDirection() const
    {
        return glm::normalize(_target - _position);
    }

    glm::vec3 Camera::GetPosition() const
    {
        return _position;
    }

    glm::vec3 Camera::GetTarget() const
    {
        return _target;
    }

    glm::vec3 Camera::GetRight() const
    {
        return glm::normalize(glm::cross(GetDirection(), glm::vec3(0, 1, 0)));
    }

    glm::vec3 Camera::GetUp() const
    {
        return glm::normalize(glm::cross(GetRight(), GetDirection()));
    }

    glm::mat4 Camera::GetLookAt() const
    {
        return glm::lookAt(_position, _target, glm::vec3(0, 1, 0));
    }

    void Camera::SetTarget(glm::vec3 target)
    {
        _target = target;
    }

    void Camera::SetPosition(glm::vec3 position)
    {
        _position = position;
    }

    glm::mat4 Camera::GetPerspective(float fov, float aspect, float near, float far) const
    {
        return glm::perspective(glm::radians(fov), aspect, near, far);
    }

    void Camera::UpdateView(ShaderProgram &shaderProgram)
    {
        _lookAt = GetLookAt();

        Uniform<glm::mat4> viewUniform("view", _lookAt, shaderProgram);
        Uniform<glm::mat4> perspectiveUniform("perspective", _perspective, shaderProgram);
    }

    void Camera::SetPerspective(float fov, float aspect, float near, float far)
    {
        _perspective = glm::perspective(glm::radians(fov), aspect, near, far);
        std::cout << glm::to_string(_perspective) << std::endl;
    }
}
