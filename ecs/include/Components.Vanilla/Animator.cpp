/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** Animator.cpp
*/

#include "ECSImpl.hpp"
#include "Components.Vanilla/CLI.hpp"
#include "Components.Vanilla/Animator.hpp"
#include "Engine.hpp"
#include "UIButton.hpp"
#include "ResourceManager.hpp"

//-----------------------------------------------------------------------------
// SPRITE
//-----------------------------------------------------------------------------

Sprite::Sprite(const std::string &path) :
    _path(path),
    _texture(LoadTexture(path.c_str())),
    _rect(Rectangle{0, 0, (float) _texture.width, (float) _texture.height}),
    _origin(Vector2{0, 0}),
    _scale(Vector2{1, 1}),
    _rotation(0),
    _color(WHITE),
    _flipX(false),
    _flipY(false),
    _visible(true)
{}

void Sprite::Draw() {
    DrawTexturePro(_texture, _rect, Rectangle{0, 0, _rect.width * _scale.x, _rect.height * _scale.y}, _origin, _rotation, _color);
}

std::string &Sprite::GetPath() {
    return _path;
}

Rectangle &Sprite::GetRect() {
    return _rect;
}

Vector2 &Sprite::GetOrigin() {
    return _origin;
}

Vector2 &Sprite::GetScale() {
    return _scale;
}

float &Sprite::GetRotation() {
    return _rotation;
}

Color &Sprite::GetColor() {
    return _color;
}

bool &Sprite::GetFlipX() {
    return _flipX;
}

bool &Sprite::GetFlipY() {
    return _flipY;
}

void Sprite::SetRect(const Rectangle &rect) {
    _rect = rect;
}

void Sprite::SetOrigin(const Vector2 &origin) {
    _origin = origin;
}

void Sprite::SetScale(const Vector2 &scale) {
    _scale = scale;
}

void Sprite::SetRotation(float rotation) {
    _rotation = rotation;
}

void Sprite::SetColor(const Color &color) {
    _color = color;
}

void Sprite::SetFlipX(bool flipX) {
    _flipX = flipX;
}

void Sprite::SetFlipY(bool flipY) {
    _flipY = flipY;
}

//-----------------------------------------------------------------------------
// FRAME
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// ANIMATION
//-----------------------------------------------------------------------------

Animation::Animation(const std::string &name) : _name(name) {}

Animation::Animation()
{
    _name = "new_animatinon_" + std::to_string(rand());
}

void Animation::AddFrame(const Frame &frame) {
    _frames.push_back(frame);
}

void Animation::ReplaceFrame(const Frame &frame, int index) {
    _frames[index] = frame;
}

Animation::Frame &Animation::GetFrame(int index) {
    return _frames[index];
}

std::vector<unsigned char> Animation::Serialize() {
    std::vector<unsigned char> data;

    ecs::ResourceManager::Serialize(_name, data);
    ecs::ResourceManager::Serialize(_frames, data);
    ecs::ResourceManager::Serialize(_sprite.GetPath(), data);
    return data;
}

void Animation::Deserialize(const std::vector<unsigned char> &data, std::vector<unsigned char>::iterator &it) {
    ecs::ResourceManager::Deserialize(data, it, _name);
    ecs::ResourceManager::Deserialize(data, it, _frames);
    std::string path;
    ecs::ResourceManager::Deserialize(data, it, path);
    _sprite = Sprite(path);
}

void Animation::SetName(const std::string &name) {
    _name = name;
}

void Animation::SetSpritePath(const std::string &path) {
    _sprite = Sprite(path);
}

void Animation::RenderFrame()
{
    if (_frames.empty())
        return;
    _sprite.SetRect(_frames[_currentFrame].rect);
    _sprite.SetOrigin(_frames[_currentFrame].origin);
    _sprite.SetScale(_frames[_currentFrame].scale);
    _sprite.SetRotation(_frames[_currentFrame].rotation);
    _sprite.SetColor(_frames[_currentFrame].color);
    _sprite.SetFlipX(_frames[_currentFrame].flipX);
    _sprite.SetFlipY(_frames[_currentFrame].flipY);
    _sprite.Draw();
    _currentFrame = (_currentFrame + 1) % _frames.size();
}

//-----------------------------------------------------------------------------
// ANIMATOR
//-----------------------------------------------------------------------------

void Animator::editorRegisterUIButtonExposed() {
    UIButton::RegisterCallback("animeditor::saveCurrAnim", [&] {
        saveCurrAnim();
    });
}

void Animator::saveCurrAnim() {
    if (_animEditorAnimationName == "") {
        Console::warn << "Couldn't save the animation because it has no name." << std::endl;
        return;
    }

}

void Animator::OnLoad() {
    if (!eng::Engine::GetEngine()->IsOptionSet("--editor"))
        return;
    Entity e = Sys.GetSystemHolder();
    CLI &cli = Sys.GetComponent<CLI>(e);

    editorRegisterUIButtonExposed();

    cli.RegisterCustomCommand("addanim", [&](__attribute__((unused)) CLI& cli, std::vector<std::string> args) {
        if (args.size() < 2)
            return;
        _registeredAnimations.push_back(args[1]);
        // TODO LOAD THE ANIMATION FROM BINARY
    });

    cli.RegisterCustomCommand("newanim", [&](__attribute__((unused)) CLI& cli, std::vector<std::string> args) {
        if (args.size() < 2)
            return;
        // todo show animation editor
    });
}
