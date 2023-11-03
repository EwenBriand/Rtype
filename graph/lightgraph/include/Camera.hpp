/*
** EPITECH PROJECT, 2023
** graph
** File description:
** Camera.hpp
*/


#pragma once

#include <string>
#include <memory>
#include <vector>
#include "Window.hpp"
#include "Uniform.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace lg {
    class Camera {
        public:
            static bool ShaderIsSetup;

            Camera();
            /**
             * @brief Sets the target of the camera.
             *
             */
            void SetTarget(glm::vec3 target);

            /**
             * @brief Sets the position of the camera.
             *
             */
            void SetPosition(glm::vec3 position);

            /**
             * @brief Returns the normalized direction of the camera.
             *
             * @return glm::vec3
             */
            glm::vec3 GetDirection() const;

            /**
             * @brief Returns the position of the camera.
             *
             * @return glm::vec3
             */
            glm::vec3 GetPosition() const;

            /**
             * @brief Returns the point the camera is looking at.
             *
             * @return glm::vec3
             */
            glm::vec3 GetTarget() const;

            /**
             * @brief Returns the Right vector of the camera.
             *
             */
            glm::vec3 GetRight() const;

            /**
             * @brief Returns the Up vector of the camera.
             *
             */
            glm::vec3 GetUp() const;

            /**
             * @brief Returns the LookAt matrix, that is the matrix which
             * serves to convert from world space to camera space.
             *
             */
            glm::mat4 GetLookAt() const;

            /**
             * @brief Returns the Perspective matrix, that is the matrix which
             * serves to convert from camera space to clip space.
             *
             */
            glm::mat4 GetPerspective(float fov, float aspect, float near, float far) const;

            /**
             * @brief Updates the view in the shader program passed in argument.
             * This function assumes that the shader program contains a uniform
             * of type mat4 named "view".
             *
             */
            void UpdateView(ShaderProgram &shaderProgram);

            /**
             * @brief Sets the values of the perspective matrix.
             *
             * @param fov : the fiewld of view in degrees
             * @param aspect : the aspect ratio of the window
             * @param near : the near clipping plane
             * @param far : the far clipping plane
             */
            void SetPerspective(float fov, float aspect, float near, float far);

        private:
            glm::vec3 _target = glm::vec3(0, 0, 1);
            glm::vec3 _position = glm::vec3(0, 0, 0);
            glm::mat4 _perspective = glm::mat4(1.0f);
            glm::mat4 _lookAt = glm::mat4(1.0f);
    };
}
