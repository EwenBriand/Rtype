/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** IGraphicalModule.hpp
*/

#ifndef DAF80547_4A0F_44B8_8650_246A0326CDE9
#define DAF80547_4A0F_44B8_8650_246A0326CDE9
#include "raylib.h"
#include <cmath>
#include <functional>
#include <string>

namespace graph {

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

        vec2<T> operator+(const vec2<T>& other) const
        {
            return { x + other.x, y + other.y };
        }

        vec2<T> operator-(const vec2<T>& other) const
        {
            return { x - other.x, y - other.y };
        }

        vec2<T> operator*(const vec2<T>& other) const
        {
            return { x * other.x, y * other.y };
        }

        vec2<T> operator/(const vec2<T>& other) const
        {
            return { x / other.x, y / other.y };
        }

        vec2<T> operator+(const T& other) const
        {
            return { x + other, y + other };
        }

        vec2<T> operator-(const T& other) const
        {
            return { x - other, y - other };
        }

        vec2<T> operator*(const T& other) const
        {
            return { x * other, y * other };
        }

        vec2<T> operator/(const T& other) const
        {
            return { x / other, y / other };
        }

        vec2<T>& operator+=(const vec2<T>& other)
        {
            x += other.x;
            y += other.y;
            return *this;
        }

        vec2<T>& operator-=(const vec2<T>& other)
        {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        vec2<T>& operator*=(const vec2<T>& other)
        {
            x *= other.x;
            y *= other.y;
            return *this;
        }

        vec2<T>& operator/=(const vec2<T>& other)
        {
            x /= other.x;
            y /= other.y;
            return *this;
        }

        vec2<T>& operator+=(const T& other)
        {
            x += other;
            y += other;
            return *this;
        }

        vec2<T>& operator-=(const T& other)
        {
            x -= other;
            y -= other;
            return *this;
        }

        vec2<T>& operator*=(const T& other)
        {
            x *= other;
            y *= other;
            return *this;
        }

        vec2<T>& operator/=(const T& other)
        {
            x /= other;
            y /= other;
            return *this;
        }

        bool operator==(const vec2<T>& other) const
        {
            return x == other.x && y == other.y;
        }

        bool operator!=(const vec2<T>& other) const
        {
            return x != other.x || y != other.y;
        }

        bool operator==(const T& other) const
        {
            return x == other && y == other;
        }

        bool operator!=(const T& other) const
        {
            return x != other || y != other;
        }

        float magnitude() const
        {
            return sqrt(pow(x, 2) + pow(y, 2));
        }

        vec2<T> normalized() const
        {
            float mag = magnitude();
            return { x / mag, y / mag };
        }

        float dot(const vec2<T>& other) const
        {
            return x * other.x + y * other.y;
        }

        float distance(const vec2<T>& other) const
        {
            return sqrt(pow(other.x - x, 2) + pow(other.y - y, 2));
        }

        vec2<T> lerp(const vec2<T>& other, float t) const
        {
            return { x + (other.x - x) * t, y + (other.y - y) * t };
        }

        vec2<T> reflect(const vec2<T>& normal) const
        {
            return *this - normal * 2 * dot(normal);
        }

        vec2<T> project(const vec2<T>& other) const
        {
            return other * (dot(other) / pow(other.magnitude(), 2));
        }

        vec2<T> rotate(float angle) const
        {
            float rad = angle * M_PI / 180;
            return { x * cos(rad) - y * sin(rad), x * sin(rad) + y * cos(rad) };
        }

        vec2<T> rotateAround(const vec2<T>& point, float angle) const
        {
            return (*this - point).rotate(angle) + point;
        }

        vec2<T> min(const vec2<T>& other) const
        {
            return { std::min(x, other.x), std::min(y, other.y) };
        }

        vec2<T> max(const vec2<T>& other) const
        {
            return { std::max(x, other.x), std::max(y, other.y) };
        }

        vec2<T> clamp(const vec2<T>& min, const vec2<T>& max) const
        {
            return this->min(max).max(min);
        }

        vec2<T> abs() const
        {
            return { std::abs(x), std::abs(y) };
        }

        vec2<T> floor() const
        {
            return { std::floor(x), std::floor(y) };
        }

        vec2<T> ceil() const
        {
            return { std::ceil(x), std::ceil(y) };
        }

        vec2<T> round() const
        {
            return { std::round(x), std::round(y) };
        }
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
        vec2f pos = { 0, 0 };
        vec2f dimensions = { 0, 0 };
        vec4uc bgColor = { 0, 0, 0, 0 };
        vec4uc borderColor = { 0, 0, 0, 0 };
        int borderSize = 0;
    } graphRect_t;

    typedef struct graphTexture_t {
        Rectangle source = { 0.0f, 0.0f, 0.0f, 0.0f };
        Rectangle dest = { 0.0f, 0.0f, 0.0f, 0.0f };
        Vector2 origin = { 0.0f, 0.0f };
        Texture2D texture;
        float rotation = 0.0f;
        Color color = { 255, 255, 255, 255 };
    } graphTexture_t;

    /**
     * @brief Describes a circle to be drawn on the window.
     *
     */
    typedef struct graphCircle_t {
        vec2f pos = { 0, 0 };
        float radius = 0;
        vec4uc color = { 0, 0, 0, 0 };
        vec4uc borderColor = { 0, 0, 0, 0 };
        float borderSize = 0;
    } graphCircle_t;

    /**
     * @brief Describes a text to be drawn on the window.
     *
     */
    typedef struct graphText_t {
        vec2f pos = { 0, 0 };
        std::string text = "";
        vec4uc color = { 0, 0, 0, 0 };
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
        virtual void Run(std::function<void(float deltaT)>&& callback) = 0;
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
        virtual void RegisterEvent(const std::string& eventKey, std::function<void()>&& callback) = 0;

        /**
         * @brief This function returns true if the event is triggered.
         *
         * @param eventKey
         * @return true
         * @return false
         */
        virtual void AddRectToBuffer(int priority, std::function<void()>&& func) = 0;

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

        /**
         * @brief This function clears the buffer of the graphical module.
         *
         */
        virtual void ClearBuffer() = 0;

        /**
         * @brief This function displays the buffer of the graphical module.
         *
         */
        virtual void DisplayBuffer() = 0;

        /**
         * @brief This function returns true if the graphical module is
         * currently running.
         *
         * @return true
         * @return false
         */
        virtual bool CheckCollisionWithRectangle(vec2f pos, Rectangle dimensions) = 0;
    };
} // namespace graph

#endif /* DAF80547_4A0F_44B8_8650_246A0326CDE9 */
