/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** AudioManager.hpp
*/

#ifndef D3B0DB9C_B659_4BE4_8F47_894357FC12F9
#define D3B0DB9C_B659_4BE4_8F47_894357FC12F9

#include "Components.Vanilla/AudioSource.hpp"
#include "Components.Vanilla/UserComponentWrapper.hpp"
#include "metadata.hpp"

/**
 * @brief displays buttons to pause the game and to quit the game
 *
 */
serialize class AudioManager : public AUserComponent {
public:
    GENERATE_METADATA(AudioManager)
    AudioManager() = default;
    ~AudioManager() = default;

    void Start() override;
    void Update(int e) override;
    void OnAddComponent(int entityID) override;

private:
    AudioSource* _audioSource;
    int _entity;
};

#endif /* D3B0DB9C_B659_4BE4_8F47_894357FC12F9 */
