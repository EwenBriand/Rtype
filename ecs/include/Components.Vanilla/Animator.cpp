/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** Animator.cpp
*/

#include "Components.Vanilla/Animator.hpp"
#include "Components.Vanilla/CLI.hpp"
#include "ECSImpl.hpp"
#include "Engine.hpp"
#include "ResourceManager.hpp"
#include "ToBytes.hpp"
#include "UIButton.hpp"
#include "UIManager.hpp"
#include <fstream>
#include <iostream>

//-----------------------------------------------------------------------------
// SPRITE
//-----------------------------------------------------------------------------

Sprite::Sprite(const std::string& path)
    : _path(path)
    , _texture(LoadTexture(path.c_str()))
    , _rect(Rectangle { 0, 0, (float)_texture.width, (float)_texture.height })
    , _origin(Vector2 { 0, 0 })
    , _scale(Vector2 { 1, 1 })
    , _rotation(0)
    , _color(WHITE)
    , _flipX(false)
    , _flipY(false)
    , _visible(true)
{
    if (_texture.id == 0)
        CONSOLE::warn << "Failed to load texture: " << path << std::endl;
}

Sprite::~Sprite()
{
    if (eng::Engine::GetEngine()->IsOptionSet(eng::Engine::Options::NO_GRAPHICS))
        return;
    UnloadTexture(_texture);
}

void Sprite::Draw()
{
    if (eng::Engine::GetEngine()->IsOptionSet(eng::Engine::Options::NO_GRAPHICS))
        return;
    if (!_visible or _texture.id == 0)
        return;
    int flipX = _flipX ? -1 : 1;
    int flipY = _flipY ? -1 : 1;
    Rectangle source = { _rect.x, _rect.y, flipX * _rect.width, flipY * _rect.height };
    Rectangle dest = { _origin.x, _origin.y, _rect.width * _scale.x, _rect.height * _scale.y };
    Vector2 origin = { _rect.width * _scale.x / 2, _rect.height * _scale.y / 2 };
    graph::graphTexture_t spriteInfo = { .source = source,
        .dest = dest,
        .origin = origin,
        .texture = _texture,
        .rotation = _rotation,
        .color = _color };
    SYS.GetGraphicalModule()->AddRectToBuffer(_priority, [spriteInfo]() {
        DrawTexturePro(spriteInfo.texture, spriteInfo.source, spriteInfo.dest, spriteInfo.origin, spriteInfo.rotation, spriteInfo.color);
    });
}

std::string& Sprite::GetPath()
{
    return _path;
}

Rectangle& Sprite::GetRect()
{
    return _rect;
}

Vector2& Sprite::GetOrigin()
{
    return _origin;
}

Vector2& Sprite::GetScale()
{
    return _scale;
}

float& Sprite::GetRotation()
{
    return _rotation;
}

Color& Sprite::GetColor()
{
    return _color;
}

bool& Sprite::GetFlipX()
{
    return _flipX;
}

bool& Sprite::GetFlipY()
{
    return _flipY;
}

void Sprite::SetRect(const Rectangle& rect)
{
    _rect = rect;
}

void Sprite::SetOrigin(const Vector2& origin)
{
    _origin = origin;
}

void Sprite::SetScale(const Vector2& scale)
{
    _scale = scale;
}

void Sprite::SetRotation(float rotation)
{
    _rotation = rotation;
}

void Sprite::SetColor(const Color& color)
{
    _color = color;
}

void Sprite::SetFlipX(bool flipX)
{
    _flipX = flipX;
}

void Sprite::SetFlipY(bool flipY)
{
    _flipY = flipY;
}

void Sprite::SetPriority(int priority)
{
    _priority = priority;
}

//-----------------------------------------------------------------------------
// FRAME
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// ANIMATION
//-----------------------------------------------------------------------------

Animation::Animation(const std::string& name)
    : _name(name)
{
}

Animation::Animation()
{
    _name = "new_animation_" + std::to_string(rand());
}

std::string& Animation::GetName()
{
    return _name;
}

void Animation::AddFrame(const Frame& frame)
{
    _frames.push_back(frame);
}

void Animation::ReplaceFrame(const Frame& frame, int index)
{
    _frames[index] = frame;
}

Animation::Frame& Animation::GetFrame(int index)
{
    return _frames[index];
}

std::vector<unsigned char> Animation::Serialize()
{
    std::vector<unsigned char> data;

    bytes::ToBytes::Serialize(_name, data);
    bytes::ToBytes::Serialize(_frames, data);
    bytes::ToBytes::Serialize(_sprite->GetPath(), data);
    return data;
}

void Animation::Deserialize(const std::vector<unsigned char>& data, std::vector<unsigned char>::iterator& it)
{
    bytes::ToBytes::Deserialize(data, it, _name);
    bytes::ToBytes::Deserialize(data, it, _frames);
    std::string path;
    bytes::ToBytes::Deserialize(data, it, path);
    _sprite = std::make_shared<Sprite>(path);
}

void Animation::SetName(const std::string& name)
{
    _name = name;
}

void Animation::SetSpritePath(const std::string& path)
{
    if (eng::Engine::GetEngine()->IsOptionSet(eng::Engine::Options::NO_GRAPHICS))
        return;
    std::string assetRoot = eng::Engine::GetEngine()->GetConfigValue("assetRoot");
    std::string spritePath = ecs::ResourceManager::MakePath({ assetRoot, path }, true);
    _sprite = std::make_shared<Sprite>(spritePath);
}

void Animation::SetParalax(bool paralax)
{
    this->paralax = paralax;
}

void Animation::SetParalaxSpeed(int paralaxSpeed)
{
    this->paralaxSpeed = paralaxSpeed;
}

void Animation::SetPriority(int priority)
{
    this->priority = priority;
}

void Animation::RenderFrame(int entityId)
{
    if (_frames.empty())
        return;
    if (_sprite)
        _sprite->SetRect(_frames[_currentFrame].rect);
    try {
        auto transform = SYS.GetComponent<CoreTransform>(entityId);
        auto x = transform.x;
        auto y = transform.y;
        if (_sprite)
            _sprite->SetOrigin({ x, y });
    } catch (std::exception& e) {
        if (_sprite)
            _sprite->SetOrigin(_frames[_currentFrame].origin);
    }
    if (_sprite) {
        _sprite->SetScale(_frames[_currentFrame].scale);
        _sprite->SetRotation(_frames[_currentFrame].rotation);
        _sprite->SetColor(_frames[_currentFrame].color);
        _sprite->SetFlipX(_frames[_currentFrame].flipX);
        _sprite->SetFlipY(_frames[_currentFrame].flipY);
        _sprite->SetPriority(priority);
        _sprite->Draw();
    }
    _frameStartTime += SYS.GetDeltaTime();
    if (_frameStartTime >= _frames[_currentFrame].duration) { // todo fix for skipping frames in case of lag
        _frameStartTime = 0;
        _currentFrame = (_currentFrame + 1) % _frames.size();
    }
}

void Animation::SetLoop(bool loop)
{
    _loop = loop;
}

bool Animation::GetParalax()
{
    return paralax;
}

int Animation::GetParalaxSpeed()
{
    return paralaxSpeed;
}

//-----------------------------------------------------------------------------
// ANIMATOR
//-----------------------------------------------------------------------------

void Animator::OnAddComponent(int entityId)
{
    for (auto name : _registeredAnimations) {
        std::string out = "";
        Animation anim = loadAnimationFromFile(name, out);
        _animations[out] = anim;
    }
}

void Animator::editorRegisterUIButtonExposed()
{
    UIButton::RegisterCallback("animeditor::saveCurrAnim", [&] {
        saveCurrAnim();
    });
}

void Animator::saveCurrAnim()
{
    if (_animEditorAnimationName == "") {
        CONSOLE::warn << "Couldn't save the animation because it has no name." << std::endl;
        return;
    }
}

void Animator::OnLoad()
{
    if (!eng::Engine::GetEngine()->IsOptionSet(eng::Engine::Options::EDITOR))
        return;
    Entity e = SYS.GetSystemHolder();
    CLI& cli = SYS.GetComponent<CLI>(e);

    editorRegisterUIButtonExposed();

    cli.RegisterCustomCommand(
        "anme", [&](__attribute__((unused)) CLI& cli, __attribute__((unused)) std::vector<std::string> args) {
            registerUIElements();
        },
        "Open the animation editor");

    cli.RegisterCustomCommand(
        "anme-q", [&](__attribute__((unused)) CLI& cli, __attribute__((unused)) std::vector<std::string> args) {
            try {
                ui::UIManager::Get().RemoveGroup(_animationEditorUIGroupHandle);
            } catch (std::exception& e) {
                CONSOLE::err << "Failed to remove ui group: " << e.what() << std::endl;
                CONSOLE::err << "This is probably because the group doesn't exist." << std::endl;
            }
        },
        "Close the animation editor");

    cli.RegisterCustomCommand(
        "anme-load", [](CLI& cli, std::vector<std::string> args) {
            if (args.size() != 2) {
                CONSOLE::warn << "Usage: anme-add <path>" << std::endl;
                return;
            }
            std::string name = "";
            Animation anim;
            Animator* animator = nullptr;
            if (cli.GetContext() == SYS.GetSystemHolder()) {
                throw std::runtime_error("Can't add animation to the system holder");
            }
            try {
                animator = &SYS.GetComponent<Animator>(cli.GetContext());
            } catch (std::exception& e) {
                SYS.AddComponent<Animator>(cli.GetContext());
                animator = &SYS.GetComponent<Animator>(cli.GetContext());
            }
            if (animator == nullptr)
                throw std::runtime_error("Failed to get animator component");
            try {
                anim = animator->loadAnimationFromFile(args[0], name);
            } catch (std::exception& e) {
                CONSOLE::err << "Failed to load animation: " << e.what() << std::endl;
                return;
            }
            animator->AddAnimation(anim.GetName(), anim);
        },
        "Add an animation to the current entity");

    cli.RegisterCustomCommand(
        "anme-play", [](CLI& cli, std::vector<std::string> args) {
            if (args.size() != 2) {
                CONSOLE::warn << "Usage: anme-play <name>" << std::endl;
                return;
            }

            try {
                Animator& animator = SYS.GetComponent<Animator>(cli.GetContext());
                animator.Play(args[0]);
            } catch (std::exception& e) {
                CONSOLE::err << "Failed to play animation: " << e.what() << std::endl;
            }
        },
        "Plays an animation from its name. The animation must be loaded first.");
}

void Animator::registerUIElements()
{
    auto& uiManager = ui::UIManager::Get();
    try {
        _animationEditorUIGroupHandle = uiManager.AddGroup(
            ui::UIManager::TextField {
                .text = "New animation",
                .placeholder = "Animation name",
                .position = Vector2 { 10, 100 },
                .fontSize = 15,
                .color = BLACK,
                .length = 20,
                .callback = [&](std::string text) {
                    _animEditorAnimationName = text;
                    if (_animations.find(text) == _animations.end())
                        createNewAnim(text);
                },
                .layer = 10 },
            ui::UIManager::Text { .text = "Animation Editor", .position = Vector2 { 10, 10 }, .fontSize = 20, .color = WHITE, .layer = 10 });
    } catch (std::exception& e) {
        CONSOLE::err << "Failed to register ui elements: " << e.what() << std::endl;
    }
}

void Animator::createNewAnim(const std::string& name)
{
    // todo try to load the animation first
    _animations[name] = Animation(name);
}

void Animator::AddAnimation(const std::string& path)
{
    std::string name = "";
    Animation anim = loadAnimationFromFile(path, name);
    _animations[name] = anim;
}

std::vector<std::string> Animator::loadRawAnimFile(const std::string& path, std::string& name)
{

    std::vector<std::string> lines;
    const std::string assetRoot = eng::Engine::GetEngine()->GetConfigValue("assetRoot");
    std::string animPath = ecs::ResourceManager::MakePath({ assetRoot, path }, true);
    std::ifstream file(animPath);
    std::string line;

    if (!file.is_open())
        throw std::runtime_error("Failed to open file: " + animPath);
    while (std::getline(file, line)) {
        line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](int ch) {
            return !std::isspace(ch);
        }));
        if (line == "" || line[0] == '#' || line[0] == '\n')
            continue;
        lines.push_back(line);
    }
    file.close();
    return lines;
}

void Animator::AddAnimation(const std::string& name, Animation& anim)
{
    std::cout << "Setting animation with name " << name << std::endl;
    _animations[name] = anim;
    for (auto& it : _animations) {
        std::cout << "Animation: " << it.first << std::endl;
    }
}

Animation Animator::loadAnimationFromFile(const std::string& path, std::string& name)
{
    std::vector<std::string> lines = loadRawAnimFile(path, name);
    decltype(lines)::iterator it = lines.begin();
    Animation anim;
    bool isFrameSection = false;
    int frameIndex = -1;
    Animation::Frame defaultFrame {
        .rect = Rectangle { 0, 0, 0, 0 },
        .origin = Vector2 { 0, 0 },
        .scale = Vector2 { 1, 1 },
        .rotation = 0,
        .color = WHITE,
        .flipX = false,
        .flipY = false,
        .duration = 1.0 / 60.0
    };

    while (it != lines.end()) {
        if (*it == "===FRAMES===") {
            isFrameSection = true;
            continue;
        }

        auto [key, value] = parse(*it);

        if (key == "auto") {
            handleAutoMode(defaultFrame, anim, value);
            name = anim.GetName();
            if (std::find(_registeredAnimations.begin(), _registeredAnimations.end(), path) == _registeredAnimations.end())
                _registeredAnimations.push_back(path);
            return anim;
        }

        if (isFrameSection) {
            if (key == "frame id") {
                anim.AddFrame(defaultFrame);
                ++frameIndex;
            } else {
                handleFrameProperties(anim, frameIndex, key, value);
            }
        } else {
            handleAnimationProperties(anim, key, value, defaultFrame);
        }

        ++it;
    }
    name = anim.GetName();
    if (std::find(_registeredAnimations.begin(), _registeredAnimations.end(), path) == _registeredAnimations.end())
        _registeredAnimations.push_back(path);
    return anim;
}

std::vector<std::string> Animator::GetRegisteredAnimations() const
{
    return _registeredAnimations;
}

void Animator::handleAutoMode(Animation::Frame& defaultFrame, Animation& anim, const std::string& value)
{
    int width = 0;
    int height = 0;
    int frameNbr = 0;
    std::istringstream iss(value);

    iss >> width >> height >> frameNbr;
    width = width / frameNbr;
    if (anim.GetParalax()) {
        frameNbr = width;
        int speed = anim.GetParalaxSpeed();
        for (int i = 0; i < frameNbr; ++i) {
            Animation::Frame frame = defaultFrame;
            int current = i;
            frame.rect = Rectangle { (float)(current), 0, (float)width, (float)height };
            frame.duration = 1.0 / (float)speed;
            anim.AddFrame(frame);
        }
    } else
        for (int i = 0; i < frameNbr; ++i) {
            Animation::Frame frame = defaultFrame;
            frame.rect = Rectangle { (float)(i * width), 0, (float)width, (float)height };
            anim.AddFrame(frame);
        }
}

std::string Animator::trim(const std::string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

std::pair<std::string, std::string> Animator::parse(const std::string& line)
{
    std::istringstream iss(line);
    std::string key = "";
    std::string value = "";

    std::getline(iss, key, ':');
    std::getline(iss, value);
    value.erase(std::remove(value.begin(), value.end(), '\n'), value.end());
    return { trim(key), trim(value) };
}

void Animator::handleAnimationProperties(Animation& anim, const std::string& key, const std::string& value, Animation::Frame& defaultFrame)
{
    if (key == "name")
        anim.SetName(value);
    else if (key == "sprite path")
        anim.SetSpritePath(value);
    else if (key == "loop")
        anim.SetLoop(value == "true");
    else if (key == "paralax")
        anim.SetParalax(value == "true");
    else if (key == "paralax speed")
        anim.SetParalaxSpeed(std::stoi(value));
    else if (key == "priority")
        anim.SetPriority(std::stoi(value));
    else if (key == "fps")
        defaultFrame.duration = 1.0 / std::stof(value);
    else if (key == "scale")
        defaultFrame.scale = Vector2 { std::stof(value), std::stof(value) };
    else if (key == "flipX")
        defaultFrame.flipX = value == "true";
    else if (key == "flipY")
        defaultFrame.flipY = value == "true";
    else
        throw std::runtime_error("Unknown animation property: " + key);
}

void Animator::handleFrameProperties(Animation& anim, int frameIndex, const std::string& key, const std::string& value)
{
    if (key == "origin") {
        std::istringstream iss(value);
        iss >> anim.GetFrame(frameIndex).origin.x >> anim.GetFrame(frameIndex).origin.y;
    } else if (key == "scale") {
        std::istringstream iss(value);
        iss >> anim.GetFrame(frameIndex).scale.x >> anim.GetFrame(frameIndex).scale.y;
    } else if (key == "rotation")
        anim.GetFrame(frameIndex).rotation = std::stof(value);
    else if (key == "flip x")
        anim.GetFrame(frameIndex).flipX = value == "true";
    else if (key == "flip y")
        anim.GetFrame(frameIndex).flipY = value == "true";
    else if (key == "color") {
        std::istringstream iss(value);
        iss >> anim.GetFrame(frameIndex).color.r >> anim.GetFrame(frameIndex).color.g >> anim.GetFrame(frameIndex).color.b >> anim.GetFrame(frameIndex).color.a;
    } else if (key == "rect") {
        std::istringstream iss(value);
        iss >> anim.GetFrame(frameIndex).rect.x >> anim.GetFrame(frameIndex).rect.y >> anim.GetFrame(frameIndex).rect.width >> anim.GetFrame(frameIndex).rect.height;
    } else {
        throw std::runtime_error("Unknown frame property: " + key);
    }
}

void Animator::Play(const std::string& name)
{
    if (_animations.find(name) == _animations.end())
        throw std::runtime_error("Animation not found: " + name);
    _currentAnimation = name;
}

void Animator::Update(int entityId)
{
    if (_currentAnimation == "")
        return;
    _animations[_currentAnimation].RenderFrame(entityId);
}
