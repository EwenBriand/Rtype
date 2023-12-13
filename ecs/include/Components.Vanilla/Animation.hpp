/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Animation.hpp
*/

#pragma once

#include "../Component.hpp"
#include "Animation.hpp"
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
    ~Sprite();
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
    std::string& GetName();

    void SetSpritePath(const std::string& path);

    void RenderFrame(int entityId);

    void SetLoop(bool loop);

private:
    std::string _name;
    std::size_t _currentFrame = 0;
    std::vector<Frame> _frames;
    bool _loop = true;
    std::shared_ptr<Sprite> _sprite;
    float _frameStartTime = 0;
};
