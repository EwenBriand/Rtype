/*
** @file AudioSource.hpp
** @brief Definition of the AudioSource class.
** @date 2023-12-11 13:55:05.
**
*/

#pragma once

#include "../Component.hpp"
#include "TextField.hpp"
#include "UIManager.hpp"
#include "metadata.hpp"
#include "raylib.h"
#include <algorithm>
#include <memory>
#include <string>
#include <vector>

serialize class AudioSource : public ecs::Component<AudioSource> {
public:
    GENERATE_METADATA(AudioSource)
    AudioSource();
    ~AudioSource();

    serialize std::vector<std::string> _music_List_Name;
    serialize std::vector<std::string> _sound_List_Name;
    std::map<std::string, std::shared_ptr<Music>> _music_List;
    std::map<std::string, std::shared_ptr<Sound>> _sound_List;

    void Update(int entityID);
    void Start();
    void OnLoad();
    void OnAddComponent(int entityID);

    void AddMusicName(const std::string& name);
    void AddSoundName(const std::string& name);

    void RmMusicName(const std::string& name);
    void RmSoundName(const std::string& name);

    void AddMusic(const std::string& name, const std::string& path);
    void AddSound(const std::string& name, const std::string& path);

    template <typename T>
    void Stop(const std::string& name);

    template <typename T>
    void Pause(const std::string& name);

    template <typename T>
    void Resume(const std::string& name);

    template <typename T>
    void Delete(const std::string& name);

    template <typename T>
    void SetVolume(const std::string& name, float volume);

    void SetGlobalVolume(float volume);

    template <typename T>
    bool IsPlaying(const std::string& name);

    float GetMusicTPlayed(const std::string& name);
    float GetMusicTLength(const std::string& name);

    template <typename T>
    void Play(const std::string& name);

private:
};
