/*
** EPITECH PROJECT, 2023
** graph
** File description:
** Window.cpp
*/

#include <iostream>
#include "Window.hpp"

namespace lg {
    Window::Window(
        int width,
        int height,
        const char *title,
        GLFWmonitor *monitor,
        GLFWwindow *share,
        long flags)
    {
        createWindow(width, height, title, monitor, share, flags);
    }

    Window::~Window()
    {
    }

/** ------------------------------------------------------------------------ */
/** --------------------------- Events ------------------------------------- */
/** ------------------------------------------------------------------------ */
    bool Window::ShouldClose()
    {
        return glfwWindowShouldClose(_window.get());
    }

    void Window::PollEvents()
    {
        glfwPollEvents();
    }

/** ------------------------------------------------------------------------ */
/** --------------------------- Getters ------------------------------------ */
/** ------------------------------------------------------------------------ */
    std::shared_ptr<GLFWwindow> Window::GetWindow() const
    {
        return _window;
    }

/** ------------------------------------------------------------------------ */
/** --------------------------- Setters ------------------------------------ */
/** ------------------------------------------------------------------------ */
    void Window::SetWindow(std::shared_ptr<GLFWwindow> window)
    {
        _window = window;
    }

/** ------------------------------------------------------------------------ */
/** --------------------------- Public methods ----------------------------- */
/** ------------------------------------------------------------------------ */

    bool Window::IsKeyPressedOnce(int key)
    {
        if (_keyPressed.find(key) == _keyPressed.end()) {
            _keyPressed[key] = false;
        }
        if (glfwGetKey(_window.get(), key) == GLFW_PRESS) {
            if (_keyPressed[key] == false) {
                _keyPressed[key] = true;
                return true;
            }
        } else {
            _keyPressed[key] = false;
        }
        return false;
    }

    bool Window::IsKeyDown(int key)
    {
        _keyPressed[key] = glfwGetKey(_window.get(), key) == GLFW_PRESS;
        return _keyPressed[key];
    }

    void Window::Draw()
    {
        glfwSwapBuffers(_window.get());
    }

    void Window::Clear(rgba color)
    {
        glClearColor(
            color.x / 255.0f,
            color.y / 255.0f,
            color.z / 255.0f,
            color.w / 255.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Window::ConvertToDeviceCoordinates(float &x, float &y)
    {
        int width;
        int height;

        glfwGetFramebufferSize(_window.get(), &width, &height);
        x = (x / width) * 2 - 1;
        y = (y / height) * 2 - 1;
    }

    void Window::Close()
    {
        glfwSetWindowShouldClose(_window.get(), true);
    }

    bool Window::IsMouseButtonDown(int button)
    {
        return glfwGetMouseButton(_window.get(), button) == GLFW_PRESS;
    }

/** ------------------------------------------------------------------------ */
/** --------------------------- Private methods ---------------------------- */
/** ------------------------------------------------------------------------ */


    void Window::createWindow(
        int width,
        int height,
        const char *title,
        GLFWmonitor *monitor,
        GLFWwindow *share,
        long flags)
    {
        if (not glfwInit())
            throw LightGraphException("Failed to initialize GLFW");

        glfwSwapInterval(1);
        glfwWindowHint(GLFW_MAXIMIZED, flags & LGWIN_MAXIMIZE_ON_CREATE);
        glfwWindowHint(GLFW_DECORATED, flags & LGWIN_DECORATED);
        glfwWindowHint(GLFW_VISIBLE, flags & LGWIN_VISIBLE);
        glfwWindowHint(GLFW_RESIZABLE, flags & LGWIN_RESIZABLE);
        glfwWindowHint(GLFW_MAXIMIZED, flags & LGWIN_MAXIMIZE_ON_CREATE);
        glfwWindowHint(GLFW_MAXIMIZED, flags & LGWIN_MAXIMIZE_ON_CREATE);
        _window = std::shared_ptr<GLFWwindow>(
            glfwCreateWindow(width, height, title, monitor, share),
            [](GLFWwindow *window) { glfwDestroyWindow(window); glfwTerminate();});
        if (not _window)
            throw LightGraphException("Failed to create GLFW window");
        std::cout << "Window created." << std::endl;
        glfwMakeContextCurrent(_window.get());
        glfwSetErrorCallback([](int error, const char *description) {
            throw LightGraphException(
                std::string("GLFW Error: ") + description);
        });
        gladLoadGL();
    }


    void Window::updateViewPort()
    {
        int width;
        int height;

        glfwGetFramebufferSize(_window.get(), &width, &height);
        glViewport(0, 0, width, height);
    }

    void Window::ToggleWireframe()
    {
        _wireframe = !_wireframe;
        glPolygonMode(GL_FRONT_AND_BACK, _wireframe ? GL_LINE : GL_FILL);
    }
}
