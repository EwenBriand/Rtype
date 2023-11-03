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

#define KEY_ENTER 257
#define KEY_BACKSPACE 259
#define KEY_DELETE 261
#define KEY_RIGHT 262
#define KEY_LEFT 263
#define KEY_DOWN 264
#define KEY_UP 265
#define KEY_ESCAPE 256
#define KEY_SPACE 32
#define KEY_TAB 258
#define KEY_LEFT_SHIFT 340
#define KEY_RIGHT_SHIFT 344
#define KEY_LEFT_CONTROL 341
#define KEY_RIGHT_CONTROL 345
#define KEY_LEFT_ALT 342
#define KEY_RIGHT_ALT 346
#define KEY_LEFT_SUPER 343
#define KEY_RIGHT_SUPER 347

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

    typedef struct graphRect_t {
        vec2f pos = {0, 0};
        vec2f dimensions = {0, 0};
        vec4uc bgColor = {0, 0, 0, 0};
        vec4uc borderColor = {0, 0, 0, 0};
        int borderSize = 0;
    } graphRect_t;

    typedef struct graphCircle_t {
        vec2f pos = {0, 0};
        float radius = 0;
        vec4uc color = {0, 0, 0, 0};
        vec4uc borderColor = {0, 0, 0, 0};
        float borderSize = 0;
    } graphCircle_t;

    typedef struct graphText_t {
        vec2f pos = {0, 0};
        std::string text = "";
        vec4uc color = {0, 0, 0, 0};
        float fontSize = 0;
    } graphText_t;

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
            virtual void WindowDrawRectangle(graphRect_t rectInfo) = 0;
            virtual void WindowDrawCircle(graphCircle_t circleInfo) = 0;
            virtual void WindowDrawText(graphText_t textInfo) = 0;

            virtual vec2f WindowGetMousePos() const = 0;
            virtual bool WindowIsMouseRightPressed() const = 0;
            virtual bool WindowIsMouseLeftPressed() const = 0;
            virtual bool WindowIsMouseRightDown() const = 0;
            virtual bool WindowIsMouseLeftDown() const = 0;
            virtual int WindowGetMouseWheelDelta() const = 0;

            virtual int GetNextCharPressed() = 0;
            virtual bool isKeyPressed(int key) = 0;

    };
} // namespace graph


#endif /* DAF80547_4A0F_44B8_8650_246A0326CDE9 */
