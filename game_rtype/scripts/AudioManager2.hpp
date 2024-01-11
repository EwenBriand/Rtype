/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** AudioManager2.hpp
*/

#pragma once

#include "Components.Vanilla/AudioSource.hpp"
#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "metadata.hpp"

/**
 * @brief displays buttons to pause the game and to quit the game
 *
 */
serialize class AudioManager2 : public AUserComponent {
public:
    GENERATE_METADATA(AudioManager2)
    AudioManager2() = default;
    ~AudioManager2() = default;

    void Start() override;
    void Update(int e) override;
    void OnAddComponent(int entityID) override;

private:
    AudioSource* _audioSource;
    TextField* _textField;
    int _entity;
};
