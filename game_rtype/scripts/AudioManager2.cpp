/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** AudioManager2.cpp
*/

#include "AudioManager2.hpp"
#include "Engine.hpp"

MANAGED_RESOURCE(AudioManager2)

void AudioManager2::OnAddComponent(int entityID)
{
    _entity = entityID;
}

void AudioManager2::Start()
{
    _audioSource = &SYS.GetComponent<AudioSource>(_entity);
    _audioSource->AddMusicName("Muse/deja_vu.ogg");
    // _audioSource->Play<Music>("Muse/deja_vu.ogg");
    std::cout << "AudioManager2::Start()" << std::endl;

    _textField = &SYS.SafeGet<TextField>(_entity);
    _textField->SetPosition({ 800, 0 });
    eng::Engine::GetEngine()->SetGlobal<int>("killCount", 0);
}

void AudioManager2::Update(int e)
{
    int killcount = eng::Engine::GetEngine()->GetGlobal<int>("killCount");
    _textField->SetText("Kill Count: " + std::to_string(killcount));
}
