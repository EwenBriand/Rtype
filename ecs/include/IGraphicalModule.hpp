/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** IGraphicalModule.hpp
*/

#ifndef DAF80547_4A0F_44B8_8650_246A0326CDE9
#define DAF80547_4A0F_44B8_8650_246A0326CDE9
#include <functional>
#include <string>
#include <cmath>
#include "raylib.h"

namespace graph
{

    template <typename T>
    struct vec3 {
        T x = 0;
        T y = 0;
        T z = 0;
    };

    template <typename T>
    struct vec2 {
        T x = 0;
        T y = 0;
    };

    template <typename T>
    struct vec4 {
        T x = 0;
        T y = 0;
        T z = 0;
        T w = 0;
    };

    template <typename T>
    float distance(vec2<T> a, vec2<T> b)
    {
        return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
    }

    template <typename T>
    float distance(vec3<T> a, vec3<T> b)
    {
        return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2) + pow(b.z - a.z, 2));
    }

    using vec2i = vec2<int>;
    using vec2f = vec2<float>;
    using vec2d = vec2<double>;
    using vec2uc = vec2<unsigned char>;

    using vec3i = vec3<int>;
    using vec3f = vec3<float>;
    using vec3d = vec3<double>;
    using vec3uc = vec3<unsigned char>;

    using vec4i = vec4<int>;
    using vec4f = vec4<float>;
    using vec4d = vec4<double>;
    using vec4uc = vec4<unsigned char>;

    /**
     * @brief Describes a rectangle to be drawn on the window.
     *
     */
    typedef struct graphRect_t {
        vec2f pos = {0, 0};
        vec2f dimensions = {0, 0};
        vec4uc bgColor = {0, 0, 0, 0};
        vec4uc borderColor = {0, 0, 0, 0};
        int borderSize = 0;
    } graphRect_t;

    /**
     * @brief Describes a circle to be drawn on the window.
     *
     */
    typedef struct graphCircle_t {
        vec2f pos = {0, 0};
        float radius = 0;
        vec4uc color = {0, 0, 0, 0};
        vec4uc borderColor = {0, 0, 0, 0};
        float borderSize = 0;
    } graphCircle_t;

    /**
     * @brief Describes a text to be drawn on the window.
     *
     */
    typedef struct graphText_t {
        vec2f pos = {0, 0};
        std::string text = "";
        vec4uc color = {0, 0, 0, 0};
        float fontSize = 0;
    } graphText_t;

    /**
     * @brief This class is the base class for all graphical modules.
     *
     */
    class IGraphicalModule {
        public:
            /**
             * @brief This functions returns the ID of the graphical module.
             * Each implementation should have a unique ID, which will serve
             * to identify the module and prevent compatibility issues.
             *
             */
            virtual std::string GetID() const = 0;
            virtual void Run(std::function<void(float deltaT)> &&callback) = 0;
            virtual void Start() = 0;
            virtual void Stop() = 0;

            /**
             * @brief This function updates the pipeline of the engine to
             * include the graphical module's actions, such as clearing the window
             * and drawing objects.
             *
             */
            virtual void ModPipeline() = 0;

            /**
             * @brief This function registers an event which will be tested for
             * every frame. If the event is triggered, the callback will be called.
             *
             * @warning if implementing a new graphical module, do not forget
             * to test for events in the pipeline!
             *
             * @param eventKey
             * @param callback
             */
            virtual void RegisterEvent(const std::string &eventKey, std::function<void()> &&callback) = 0;

            /**
             * @brief This function buffers a rectangle to be drawn in the window.
             *
             * @param rectInfo
             */
            virtual void WindowDrawRectangle(graphRect_t rectInfo) = 0;

            /**
             * @brief This function buffers a circle to be drawn in the window.
             *
             * @param circleInfo
             */
            virtual void WindowDrawCircle(graphCircle_t circleInfo) = 0;

            /**
             * @brief This function buffers a text to be drawn in the window.
             *
             * @param textInfo
             */
            virtual void WindowDrawText(graphText_t textInfo) = 0;

            /**
             * @brief This function returns a vector2f containing the mouse position on the screen
             *
             * @return vec2f
             */
            virtual vec2f WindowGetMousePos() const = 0;

            /**
             * @brief This function returns true if the mouse right button  has been pressed
             * during the current frame.
             *
             * @return true
             * @return false
             */
            virtual bool WindowIsMouseRightPressed() const = 0;

            /**
             * @brief This function returns true if the mouse left button has
             * been pressed during the current frame.
             *
             * @return true
             * @return false
             */
            virtual bool WindowIsMouseLeftPressed() const = 0;

            /**
             * @brief This function returns true if the mouse right button is
             * actively being pressed down.
             *
             * @return true
             * @return false
             */
            virtual bool WindowIsMouseRightDown() const = 0;

            /**
             * @brief This function returns true if the mouse left button is
             * actively being pressed down.
             *
             * @return true
             * @return false
             */
            virtual bool WindowIsMouseLeftDown() const = 0;

            /**
             * @brief Returns an integer representing the mouse wheel delta, that is
             * by how much the mouse wheel has been scrolled.
             *
             * @return int
             */
            virtual int WindowGetMouseWheelDelta() const = 0;

            /**
             * @brief Generator that returns the next char in the list of the
             * characters that have been pressed during the current frame.
             *
             * @return int
             */
            virtual int GetNextCharPressed() = 0;

            /**
             * @brief Returns true if the specified key is actively being pressed down.
             *
             * @param key
             * @return true
             * @return false
             */
            virtual bool isKeyPressed(int key) = 0;

    };
} // namespace graph


#endif /* DAF80547_4A0F_44B8_8650_246A0326CDE9 */
