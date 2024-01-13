/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** AudioManager.cpp
*/

#include "AudioManager.hpp"
#include "Engine.hpp"

MANAGED_RESOURCE(AudioManager)

void AudioManager::OnAddComponent(int entityID)
{
    _entity = entityID;
}

void AudioManager::Start()
{
    _audioSource = &SYS.GetComponent<AudioSource>(_entity);
    _audioSource->AddMusicName("Muse/gas_gas_gas.ogg");
    // _audioSource->Play<Music>("Muse/gas_gas_gas.ogg");
    std::cout << "AudioManager::Start()" << std::endl;

    _textField = &SYS.GetComponent<TextField>(_entity);
    _textField->SetPosition({ 800, 0 });
    eng::Engine::GetEngine()->SetGlobal<int>("killCount", 0);
}

void AudioManager::Update(int e)
{
    int killcount = eng::Engine::GetEngine()->GetGlobal<int>("killCount");
    _textField->SetText("Kill Count: " + std::to_string(killcount));
}
