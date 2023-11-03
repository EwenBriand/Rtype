/*
** EPITECH PROJECT, 2023
** graph
** File description:
** Window.hpp
*/

#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glad.h"
#include <memory>
#include <stdexcept>
#include <functional>
#include <map>
#include "VAO.hpp"

namespace lg
{
    // template <typename T>
    // struct vec2 {
    //     T x;
    //     T y;
    // };

    // template <typename T>
    // struct vec3 {
    //     T x;
    //     T y;
    //     T z;
    // };

    // template <typename T>
    // struct vec4 {
    //     T x;
    //     T y;
    //     T z;
    //     T w;
    // };

    // using vec2f = vec2<float>;
    // using vec2i = vec2<int>;
    // using vec2u = vec2<unsigned int>;

    // using vec3f = vec3<float>;
    // using vec3i = vec3<int>;
    // using vec3u = vec3<unsigned int>;

    // using vec4f = vec4<float>;
    // using vec4i = vec4<int>;
    // using vec4u = vec4<unsigned int>;
    // using rgba = vec4<unsigned char>;

    using vec2f = glm::vec2;
    using vec2i = glm::ivec2;
    using vec2u = glm::uvec2;

    using vec3f = glm::vec3;
    using vec3i = glm::ivec3;
    using vec3u = glm::uvec3;

    using vec4f = glm::vec4;
    using vec4i = glm::ivec4;
    using vec4u = glm::uvec4;
    using rgba = glm::vec4;


    class LightGraphException : public std::exception {
        public:
            LightGraphException(const std::string &message) : _message(message) {}

            const char *what() const noexcept override
            {
                return _message.c_str();
            }

        private:
            std::string _message;
    };

    /**
     * @brief This enum is used to set the window hints, and various options.
     *
     */
    enum WinOpsFlag {
        LGWIN_FALSE = 0,
        LGWIN_MAXIMIZE_ON_CREATE = 1 << 0,
        LGWIN_FULLSCREEN_ON_CREATE = 1 << 1,
        LGWIN_RESIZABLE = 1 << 2,
        LGWIN_VISIBLE = 1 << 3,
        LGWIN_DECORATED = 1 << 4,
    };


    /**
     * @brief This class encapsulates the GLFWwindow object.
     * It provides a simple interface to create a window and
     * to handle events.
     *
     * It also provides an interface to create buffer objects and
     * shaders.
     *
     */
    class Window {
        public:
            Window(
                int width = 800,
                int height = 600,
                const char *title = "LightGraph Window",
                GLFWmonitor *monitor = nullptr,
                GLFWwindow *share = nullptr,
                long flags = LGWIN_VISIBLE | LGWIN_DECORATED
            );

            ~Window();

/** ------------------------------------------------------------------------ */
/** --------------------------- Events ------------------------------------- */
/** ------------------------------------------------------------------------ */
            /**
             * @brief This function is used to check if the window should close.
             *
             * @return true if the window should close.
             * @return false if the window should not close.
             */
            bool ShouldClose();

            /**
             * @brief This function is used to poll the events.
             *
             */
            void PollEvents();

/** ------------------------------------------------------------------------ */
/** --------------------------- Getters ------------------------------------ */
/** ------------------------------------------------------------------------ */
            /**
             * @brief This function is used to get the window.
             *
             * @return std::shared_ptr<GLFWwindow> the window.
             */
            std::shared_ptr<GLFWwindow> GetWindow() const;

/** ------------------------------------------------------------------------ */
/** --------------------------- Setters ------------------------------------ */
/** ------------------------------------------------------------------------ */
            /**
             * @brief This function is used to set the window.
             *
             * @warning This function is not recommended to be used unless
             * you know what you are doing.
             *
             * @param window the window.
             */
            void SetWindow(std::shared_ptr<GLFWwindow> window);

/** ------------------------------------------------------------------------ */
/** --------------------------- Public methods ----------------------------- */
/** ------------------------------------------------------------------------ */
        /**
         * @brief This function returns true if the key is pressed this frame but
         * not the previous one.
        */
        bool IsKeyPressedOnce(int keycode);

        /**
         * @brief This function returns true if the key is in its pressed state.
        */
        bool IsKeyDown(int keycode);

        /**
         * @brief Draws the rendered buffer on the screen.
         *
         */
        void Draw();

        /**
         * @brief Clears the screen.
         *
        */
        void Clear(rgba color = {0, 0, 0, 255});

        /**
         * @brief This function is used to convert the coordinates from
         * the window to the device coordinates, between -1 and 1.
         *
         * @param x
         * @param y
         */
        void ConvertToDeviceCoordinates(float &x, float &y);

        /**
         * @brief Toggles the wireframe mode. Polygons will be displayed
         * as wireframes.
         *
         */
        void ToggleWireframe();

        /**
         * @brief This function closes the window.
         *
         */
        void Close();

        /**
         * @brief Returns true if the specified mouse button is being pressed down, false
         * otherwise.
         * @param button : the code for the mouse button, which can be either GLFW_MOUSE_BUTTON_LEFT,
         * GLFW_MOUSE_BUTTON_RIGHT or GLFW_MOUSE_BUTTON_MIDDLE.
         */
        bool IsMouseButtonDown(int button);

/** ------------------------------------------------------------------------ */
/** --------------------------- Private methods ---------------------------- */
/** ------------------------------------------------------------------------ */
        private:
            /**
             * @brief Create a Window object. This function is private because
             * it is only used in the constructor.
             *
             * @param width
             * @param height
             * @param title
             * @param monitor
             * @param share
             * @param flags
             */
            void createWindow(
                int width = 800,
                int height = 600,
                const char *title = "LightGraph Window",
                GLFWmonitor *monitor = nullptr,
                GLFWwindow *share = nullptr,
                long flags = LGWIN_VISIBLE | LGWIN_DECORATED
            );

            /**
             * @brief Updates the viewPort of OpenGL to match the current
             * window size.
             *
             * This function is useful because opengl's coordinates are
             * normalized between -1 and 1. Using this function allows
             * to display on the full window.
             *
             */
            void updateViewPort();
/** ------------------------------------------------------------------------ */
/** --------------------------- Members ------------------------------------ */
/** ------------------------------------------------------------------------ */

            std::shared_ptr<GLFWwindow> _window;
            std::map<int, bool> _keyPressed;
            bool _wireframe = false;
    };
} // namespace lg
