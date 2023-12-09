/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** Animator.hpp
*/

#ifndef ANIMATOR_HPP
#define ANIMATOR_HPP
#include "../Component.hpp"
#include "TextField.hpp"
#include "UIManager.hpp"
#include "metadata.hpp"
#include "raylib.h"
#include <functional>
#include <map>
#include <string>
#include <vector>

class Sprite {
public:
    Sprite(const std::string& path);
    Sprite() = default;
    void Draw();

    Rectangle& GetRect();
    Vector2& GetOrigin();
    Vector2& GetScale();
    float& GetRotation();
    Color& GetColor();
    bool& GetFlipX();
    bool& GetFlipY();
    std::string& GetPath();

    void SetRect(const Rectangle& rect);
    void SetOrigin(const Vector2& origin);
    void SetScale(const Vector2& scale);
    void SetRotation(float rotation);
    void SetColor(const Color& color);
    void SetFlipX(bool flipX);
    void SetFlipY(bool flipY);

private:
    std::string _path;
    Texture2D _texture;
    Rectangle _rect;
    Vector2 _origin;
    Vector2 _scale;
    float _rotation;
    Color _color;
    bool _flipX;
    bool _flipY;
    bool _visible;
};

class Animation {
public:
    struct Frame {
        Rectangle rect;
        Vector2 origin;
        Vector2 scale;
        float rotation;
        Color color;
        bool flipX;
        bool flipY;
        float duration;
    };

    Animation(const std::string& name);
    Animation();

    void AddFrame(const Frame& frame);
    void ReplaceFrame(const Frame& frame, int index);
    Frame& GetFrame(int index);

    std::vector<unsigned char> Serialize();
    void Deserialize(const std::vector<unsigned char>& data, std::vector<unsigned char>::iterator& it);

    void SetName(const std::string& name);

    void SetSpritePath(const std::string& path);

    void RenderFrame();

private:
    std::string _name;
    std::size_t _currentFrame = 0;
    std::vector<Frame> _frames;
    Sprite _sprite;
};

serialize class Animator : public ecs::Component<Animator> {
public:
    GENERATE_METADATA(Animator);

    void OnLoad();

    void Play(const std::string& name);

private:
    /**
     * @brief Register ui elements to the ui manager to display the animation
     * editor.
     *
     */
    void registerUIElements();

    /**
     * @brief Exposes a few callbacks to be callables from buttons
     *
     */
    void editorRegisterUIButtonExposed();

    /**
     * @brief Saves the current entity to a file. If the entity hasn't been
     * given a name, it doesn't save and outputs a warning in the console.
     *
     */
    void saveCurrAnim();

    /**
     * @brief Creates a new animation and sets it as the current animation.
     *
     */
    void createNewAnim(const std::string& name);

    serialize std::string _currentAnimation = "";
    serialize std::vector<std::string> _registeredAnimations;

    std::map<std::string, Animation> _animations;

    // std::map<std::string, Animation> _animations;

    std::string _animEditorAnimationName = "";

    std::size_t _animationEditorUIGroupHandle = -1;
};

#endif // ANIMATOR_HPP
