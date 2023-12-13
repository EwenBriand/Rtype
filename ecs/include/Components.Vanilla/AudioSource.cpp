/*
** @file AudioSource.cpp
** @brief Implementation of the AudioSource class.
** @date 2023-12-11 13:55:05.
**
*/

#include "AudioSource.hpp"
#include "Components.Vanilla/Animator.hpp"
#include "Components.Vanilla/CLI.hpp"
#include "ECSImpl.hpp"
#include "Engine.hpp"
#include "ResourceManager.hpp"
#include "ToBytes.hpp"
#include "UIButton.hpp"
#include "UIManager.hpp"

AudioSource::~AudioSource()
{
}

AudioSource::AudioSource()
{
}

void AudioSource::Update(int entityID)
{
    static float timePlayed = 0.0f;
    for (auto& [name, music] : _music_List) {
        if (IsMusicReady(*music) == false)
            continue;
        UpdateMusicStream(*music);

        timePlayed = GetMusicTimePlayed(*music) / GetMusicTimeLength(*music);

        if (timePlayed > 1.0f)
            timePlayed = 1.0f;
    }
}

void AudioSource::Start()
{
}

void AudioSource::OnAddComponent(int entityID)
{
    for (auto& name : _music_List_Name)
        AddMusic(name, name);
    for (auto& name : _sound_List_Name)
        AddSound(name, name);
}

void AudioSource::AddMusicName(const std::string& name)
{
    if (std::find(_music_List_Name.begin(), _music_List_Name.end(), name) != _music_List_Name.end())
        return;
    _music_List_Name.push_back(name);
    if (IsAudioDeviceReady() == false) {
        CONSOLE::err << "Error in initializing audio device" << std::endl;
    } else
        std::cout << "Audio device initialized" << std::endl;

    std::string assetRoot = eng::Engine::GetEngine()->GetConfigValue("assetRoot");
    std::string spritePath = ecs::ResourceManager::MakePath({ assetRoot, name }, true);
    AddMusic(name, spritePath);
}

void AudioSource::AddSoundName(const std::string& name)
{
    if (std::find(_sound_List_Name.begin(), _sound_List_Name.end(), name) != _sound_List_Name.end())
        return;
    _sound_List_Name.push_back(name);

    std::string assetRoot = eng::Engine::GetEngine()->GetConfigValue("assetRoot");
    std::string spritePath = ecs::ResourceManager::MakePath({ assetRoot, name }, true);
    AddSound(name, spritePath);
}

template <>
void AudioSource::Play<Music>(const std::string& name)
{
    if (_music_List.find(name) == _music_List.end())
        return;
    PlayMusicStream(*_music_List.at(name));
}

template <>
void AudioSource::Play<Sound>(const std::string& name)
{
    if (_sound_List.find(name) == _sound_List.end())
        return;
    PlaySound(*_sound_List.at(name));
}

template <>
void AudioSource::Stop<Music>(const std::string& name)
{
    if (_music_List.find(name) == _music_List.end())
        return;
    StopMusicStream(*_music_List.at(name));
}

template <>
void AudioSource::Stop<Sound>(const std::string& name)
{
    if (_sound_List.find(name) == _sound_List.end())
        return;
    StopSound(*_sound_List.at(name));
}

template <>
void AudioSource::Pause<Music>(const std::string& name)
{
    if (_music_List.find(name) == _music_List.end())
        return;
    PauseMusicStream(*_music_List.at(name));
}

template <>
void AudioSource::Pause<Sound>(const std::string& name)
{
    if (_sound_List.find(name) == _sound_List.end())
        return;
    PauseSound(*_sound_List.at(name));
}

template <>
void AudioSource::Resume<Music>(const std::string& name)
{
    if (_music_List.find(name) == _music_List.end())
        return;
    ResumeMusicStream(*_music_List.at(name));
}

template <>
void AudioSource::Resume<Sound>(const std::string& name)
{
    if (_sound_List.find(name) == _sound_List.end())
        return;
    ResumeSound(*_sound_List.at(name));
}

template <>
void AudioSource::Delete<Music>(const std::string& name)
{
    if (_music_List.find(name) == _music_List.end())
        return;
    UnloadMusicStream(*_music_List.at(name));
    _music_List.erase(name);
}

template <>
void AudioSource::Delete<Sound>(const std::string& name)
{
    if (_sound_List.find(name) == _sound_List.end())
        return;
    UnloadSound(*_sound_List.at(name));
    _sound_List.erase(name);
}

template <>
void AudioSource::SetVolume<Music>(const std::string& name, float volume)
{
    if (_music_List.find(name) == _music_List.end())
        return;
    SetMusicVolume(*_music_List.at(name), volume);
}

template <>
void AudioSource::SetVolume<Sound>(const std::string& name, float volume)
{
    if (_sound_List.find(name) == _sound_List.end())
        return;
    SetSoundVolume(*_sound_List.at(name), volume);
}

void AudioSource::SetGlobalVolume(float volume)
{
    SetMasterVolume(volume);
}

template <>
bool AudioSource::IsPlaying<Music>(const std::string& name)
{
    if (_music_List.find(name) == _music_List.end())
        return false;
    return IsMusicStreamPlaying(*_music_List.at(name));
}

template <>
bool AudioSource::IsPlaying<Sound>(const std::string& name)
{
    if (_sound_List.find(name) == _sound_List.end())
        return false;
    return IsSoundPlaying(*_sound_List.at(name));
}

void AudioSource::AddMusic(const std::string& name, const std::string& path)
{
    if (_music_List.find(name) != _music_List.end())
        return;
    Music tmp = LoadMusicStream(path.c_str());
    if (tmp.ctxData == NULL)
        _music_List_Name.erase(std::remove(_music_List_Name.begin(), _music_List_Name.end(), name), _music_List_Name.end());
    else
        _music_List[name] = std::make_shared<Music>(tmp);
}

void AudioSource::AddSound(const std::string& name, const std::string& path)
{
    if (_sound_List.find(name) != _sound_List.end())
        return;
    Sound tmp = LoadSound(path.c_str());
    if (tmp.frameCount == 0)
        _sound_List_Name.erase(std::remove(_sound_List_Name.begin(), _sound_List_Name.end(), name), _sound_List_Name.end());
    else
        _sound_List[name] = std::make_shared<Sound>(tmp);
}

float AudioSource::GetMusicTPlayed(const std::string& name)
{
    if (_music_List.find(name) == _music_List.end())
        return 0.0f;
    return GetMusicTimePlayed(*_music_List.at(name));
}

float AudioSource::GetMusicTLength(const std::string& name)
{
    if (_music_List.find(name) == _music_List.end())
        return 0.0f;
    return GetMusicTimeLength(*_music_List.at(name));
}

void AudioSource::OnLoad()
{

    if (!eng::Engine::GetEngine()->IsOptionSet(eng::Engine::Options::EDITOR))
        return;
    Entity e = SYS.GetSystemHolder();
    CLI& cli = SYS.GetComponent<CLI>(e);

    cli.RegisterCustomCommand(
        "AddMusic", [&](__attribute__((unused)) CLI& cli, std::vector<std::string> args) {
            if (args.size() != 2) {
                std::cout << "Usage: AddMusic <path>" << std::endl;
                return;
            }
            try {
                AudioSource& audio = SYS.SafeGet<AudioSource>(cli);
                audio.AddMusicName(args[0]);
            } catch (std::exception& e) {
                CONSOLE::err << "Error in adding a music : " << e.what() << std::endl;
            }
        },
        "Add a music");

    cli.RegisterCustomCommand(
        "AddSound", [&](__attribute__((unused)) CLI& cli, std::vector<std::string> args) {
            if (args.size() != 2) {
                std::cout << "Usage: AddSound <path>" << std::endl;
                return;
            }
            try {
                AudioSource& audio = SYS.SafeGet<AudioSource>(cli);
                audio.AddSoundName(args[0]);
            } catch (std::exception& e) {
                CONSOLE::err << "Error in adding a sound : " << e.what() << std::endl;
            }
        },
        "Add a sound");

    cli.RegisterCustomCommand(
        "PlayMusic", [&](__attribute__((unused)) CLI& cli, std::vector<std::string> args) {
            if (args.size() != 2) {
                std::cout << "Usage: PlayMusic <name>" << std::endl;
                return;
            }
            try {
                AudioSource& audio = SYS.SafeGet<AudioSource>(cli);
                audio.Play<Music>(args[0]);
            } catch (std::exception& e) {
                CONSOLE::err << "Error in playing a music : " << e.what() << std::endl;
            }
        },
        "Play a music");

    cli.RegisterCustomCommand(
        "PlaySound", [&](__attribute__((unused)) CLI& cli, std::vector<std::string> args) {
            if (args.size() != 2) {
                std::cout << "Usage: PlaySound <name>" << std::endl;
                return;
            }
            try {
                AudioSource& audio = SYS.SafeGet<AudioSource>(cli);
                audio.Play<Sound>(args[0]);
            } catch (std::exception& e) {
                CONSOLE::err << "Error in playing a sound : " << e.what() << std::endl;
            }
        },
        "Play a sound");

    cli.RegisterCustomCommand(
        "StopMusic", [&](__attribute__((unused)) CLI& cli, std::vector<std::string> args) {
            if (args.size() != 2) {
                std::cout << "Usage: StopMusic <name>" << std::endl;
                return;
            }
            try {
                AudioSource& audio = SYS.SafeGet<AudioSource>(cli);
                audio.Stop<Music>(args[0]);
            } catch (std::exception& e) {
                CONSOLE::err << "Error in stopping a music : " << e.what() << std::endl;
            }
        },
        "Stop a music");

    cli.RegisterCustomCommand(
        "StopSound", [&](__attribute__((unused)) CLI& cli, std::vector<std::string> args) {
            if (args.size() != 2) {
                std::cout << "Usage: StopSound <name>" << std::endl;
                return;
            }
            try {
                AudioSource& audio = SYS.SafeGet<AudioSource>(cli);
                audio.Stop<Sound>(args[0]);
            } catch (std::exception& e) {
                CONSOLE::err << "Error in stopping a sound : " << e.what() << std::endl;
            }
        },
        "Stop a sound");

    cli.RegisterCustomCommand(
        "PauseMusic", [&](__attribute__((unused)) CLI& cli, std::vector<std::string> args) {
            if (args.size() != 2) {
                std::cout << "Usage: PauseMusic <name>" << std::endl;
                return;
            }
            try {
                AudioSource& audio = SYS.SafeGet<AudioSource>(cli);
                audio.Pause<Music>(args[0]);
            } catch (std::exception& e) {
                CONSOLE::err << "Error in pausing a music : " << e.what() << std::endl;
            }
        },
        "Pause a music");

    cli.RegisterCustomCommand(
        "PauseSound", [&](__attribute__((unused)) CLI& cli, std::vector<std::string> args) {
            if (args.size() != 2) {
                std::cout << "Usage: PauseSound <name>" << std::endl;
                return;
            }
            try {
                AudioSource& audio = SYS.SafeGet<AudioSource>(cli);
                audio.Pause<Sound>(args[0]);
            } catch (std::exception& e) {
                CONSOLE::err << "Error in pausing a sound : " << e.what() << std::endl;
            }
        },
        "Pause a sound");

    cli.RegisterCustomCommand(
        "ResumeMusic", [&](__attribute__((unused)) CLI& cli, std::vector<std::string> args) {
            if (args.size() != 2) {
                std::cout << "Usage: ResumeMusic <name>" << std::endl;
                return;
            }
            try {
                AudioSource& audio = SYS.SafeGet<AudioSource>(cli);
                audio.Resume<Music>(args[0]);
            } catch (std::exception& e) {
                CONSOLE::err << "Error in resuming a music : " << e.what() << std::endl;
            }
        },
        "Resume a music");

    cli.RegisterCustomCommand(
        "ResumeSound", [&](__attribute__((unused)) CLI& cli, std::vector<std::string> args) {
            if (args.size() != 2) {
                std::cout << "Usage: ResumeSound <name>" << std::endl;
                return;
            }
            try {
                AudioSource& audio = SYS.SafeGet<AudioSource>(cli);
                audio.Resume<Sound>(args[0]);
            } catch (std::exception& e) {
                CONSOLE::err << "Error in resuming a sound : " << e.what() << std::endl;
            }
        },
        "Resume a sound");

    cli.RegisterCustomCommand(
        "DeleteMusic", [&](__attribute__((unused)) CLI& cli, std::vector<std::string> args) {
            if (args.size() != 2) {
                std::cout << "Usage: DeleteMusic <name>" << std::endl;
                return;
            }
            try {
                AudioSource& audio = SYS.SafeGet<AudioSource>(cli);
                audio.Delete<Music>(args[0]);
            } catch (std::exception& e) {
                CONSOLE::err << "Error in deleting a music : " << e.what() << std::endl;
            }
        },
        "Delete a music");

    cli.RegisterCustomCommand(
        "DeleteSound", [&](__attribute__((unused)) CLI& cli, std::vector<std::string> args) {
            if (args.size() != 2) {
                std::cout << "Usage: DeleteSound <name>" << std::endl;
                return;
            }
            try {
                AudioSource& audio = SYS.SafeGet<AudioSource>(cli);
                audio.Delete<Sound>(args[0]);
            } catch (std::exception& e) {
                CONSOLE::err << "Error in deleting a sound : " << e.what() << std::endl;
            }
        },
        "Delete a sound");

    cli.RegisterCustomCommand(
        "SetVolumeMusic", [&](__attribute__((unused)) CLI& cli, std::vector<std::string> args) {
            if (args.size() != 3) {
                std::cout << "Usage: SetVolumeMusic <name> <volume>" << std::endl;
                return;
            }
            try {
                AudioSource& audio = SYS.SafeGet<AudioSource>(cli);
                audio.SetVolume<Music>(args[0], std::stof(args[1]));
            } catch (std::exception& e) {
                CONSOLE::err << "Error in setting volume of a music : " << e.what() << std::endl;
            }
        },
        "Set the volume of a music");

    cli.RegisterCustomCommand(
        "SetVolumeSound", [&](__attribute__((unused)) CLI& cli, std::vector<std::string> args) {
            if (args.size() != 3) {
                std::cout << "Usage: SetVolumeSound <name> <volume>" << std::endl;
                return;
            }
            try {
                AudioSource& audio = SYS.SafeGet<AudioSource>(cli);
                audio.SetVolume<Sound>(args[0], std::stof(args[1]));
            } catch (std::exception& e) {
                CONSOLE::err << "Error in setting volume of a sound : " << e.what() << std::endl;
            }
        },
        "Set the volume of a sound");

    cli.RegisterCustomCommand(
        "SetGlobalVolume", [&](__attribute__((unused)) CLI& cli, std::vector<std::string> args) {
            if (args.size() != 2) {
                std::cout << "Usage: SetGlobalVolume <volume>" << std::endl;
                return;
            }
            try {
                AudioSource& audio = SYS.SafeGet<AudioSource>(cli);
                audio.SetGlobalVolume(std::stof(args[0]));
            } catch (std::exception& e) {
                CONSOLE::err << "Error in setting global volume : " << e.what() << std::endl;
            }
        },
        "Set the global volume");

    cli.RegisterCustomCommand(
        "IsPlayingMusic", [&](__attribute__((unused)) CLI& cli, std::vector<std::string> args) {
            if (args.size() != 2) {
                std::cout << "Usage: IsPlayingMusic <name>" << std::endl;
                return;
            }
            try {
                AudioSource& audio = SYS.SafeGet<AudioSource>(cli);
                std::cout << audio.IsPlaying<Music>(args[0]) << std::endl;
            } catch (std::exception& e) {
                CONSOLE::err << "Error in checking if a music is playing : " << e.what() << std::endl;
            }
        },
        "Check if a music is playing");

    cli.RegisterCustomCommand(
        "IsPlayingSound", [&](__attribute__((unused)) CLI& cli, std::vector<std::string> args) {
            if (args.size() != 2) {
                std::cout << "Usage: IsPlayingSound <name>" << std::endl;
                return;
            }
            try {
                AudioSource& audio = SYS.SafeGet<AudioSource>(cli);
                std::cout << audio.IsPlaying<Sound>(args[0]) << std::endl;
            } catch (std::exception& e) {
                CONSOLE::err << "Error in checking if a sound is playing : " << e.what() << std::endl;
            }
        },
        "Check if a sound is playing");

    cli.RegisterCustomCommand(
        "GetMusicTPlayed", [&](__attribute__((unused)) CLI& cli, std::vector<std::string> args) {
            if (args.size() != 2) {
                std::cout << "Usage: GetMusicTPlayed <name>" << std::endl;
                return;
            }
            try {
                AudioSource& audio = SYS.SafeGet<AudioSource>(cli);
                std::cout << audio.GetMusicTPlayed(args[0]) << std::endl;
            } catch (std::exception& e) {
                CONSOLE::err << "Error in getting time played of a music : " << e.what() << std::endl;
            }
        },
        "Get the time played of a music");

    cli.RegisterCustomCommand(
        "GetMusicTLength", [&](__attribute__((unused)) CLI& cli, std::vector<std::string> args) {
            if (args.size() != 2) {
                std::cout << "Usage: GetMusicTLength <name>" << std::endl;
                return;
            }
            try {
                AudioSource& audio = SYS.SafeGet<AudioSource>(cli);
                std::cout << audio.GetMusicTLength(args[0]) << std::endl;
            } catch (std::exception& e) {
                CONSOLE::err << "Error in getting time length of a music : " << e.what() << std::endl;
            }
        },
        "Get the time length of a music");
}
