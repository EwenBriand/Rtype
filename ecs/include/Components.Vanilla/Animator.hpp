/*
** EPITECH PROJECT, 2023
** metadata
** File description:
** Animator.hpp
*/

#ifndef ANIMATOR_HPP
#define ANIMATOR_HPP
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

// spaceship assets: https://maxparata.itch.io/voxel-spaceships?download
// http://kuut.xyz/spritesheet/

serialize class Animator : public ecs::Component<Animator> {
public:
    GENERATE_METADATA(Animator);

    void AddAnimation(const std::string& path);
    void AddAnimation(const std::string& name, Animation& anim);

    void OnLoad();
    void OnAddComponent(int entityId);
    void Update(int entityId);

    /**
     * @brief Renders the current animation.
     *
     */
    void Play(const std::string& name);

    static std::map<std::string, std::function<void(Animation&, std::vector<std::string>::iterator& it)>> LoadAnimationFromFileCallbacks;

    std::vector<std::string> GetRegisteredAnimations() const;

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

    /**
     * @brief Loads a single animation from a file.
     * The specified path are relative to the assetRoot specified in the
     * base.cfg file.
     * The file format is as follows: (brace brackets shouldn't be written, and the keys should be written in the order they are specified)
     *
     * [name]: string                   // name of the animation
     * [sprite path]: string            // path to the sprite
     * [loop]: bool                     // whether the animation loops or not
     * [fps]: int                       // fps of the animation
     * [scale]: float, float            // scale of the sprite
     * [auto]: int, int, int            // wether the rect of the sprite should be automatically calculated or not. This works only if all the sprites are the same size and are aligned on a line. The parameters are: width, height, number of sprites
     * ===FRAMES===                     // keyword to indicate the start of the frames specification
     * [frame id] {                     // frames, each scope is one frame
     *      [origin]: float, float      // (optional) origin of the sprite, overrides the default origin
     *      [scale]: float, float       // (optional) scale of the sprite, overrides the default scale
     *      [rotation]: float           // (optional) rotation of the sprite, overrides the default rotation
     *      [color]: int, int, int, int // (optional) color of the sprite, overrides the default color
     * }
     * [frame id] { ... }
     * ...
     *
     * @param path
     * @param name : out parameter
     * @return Animation
     */
    Animation loadAnimationFromFile(const std::string& path, std::string& name);
    std::vector<std::string> loadRawAnimFile(const std::string& path, std::string& name);
    std::string trim(const std::string& str);
    std::pair<std::string, std::string> parse(const std::string& line);
    void handleAnimationProperties(Animation& anim, const std::string& key, const std::string& value, Animation::Frame& defaultFrame);
    void handleFrameProperties(Animation& anim, int frameIndex, const std::string& key, const std::string& value);
    void handleAutoMode(Animation::Frame& defaultFrame, Animation& anim, const std::string& value);

    std::map<std::string, Animation> _animations;

    std::string _animEditorAnimationName = "";

    std::size_t _animationEditorUIGroupHandle = -1;
};

#endif // ANIMATOR_HPP
