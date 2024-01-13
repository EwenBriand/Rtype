/*
* CAUTION: This file was generated automatically.
* Do not modify it, as your changes will be lost.
*/

#include <sstream>
#include "ToBytes.hpp"
#include <iostream>
#include "metadata.hpp"
#include "AudioSource.hpp"

void AudioSource::Load(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Load(filepath, _music_List_Name, _sound_List_Name);
}

void AudioSource::Save(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Save(filepath, _music_List_Name, _sound_List_Name);
}

std::map<std::string, metadata_t> AudioSource::GetMetadata()
{
	std::map<std::string, metadata_t> metadata;
	metadata["_music_List_Name"] = {"std::vector<std::string>", "_music_List_Name", sizeof(std::vector<std::string>), [&](const std::string &str) {std::stringstream _music_List_Name_ss(str);std::string _music_List_Name_value;while (getline(_music_List_Name_ss, _music_List_Name_value, ',')) {_music_List_Name.push_back(fromString<decltype(_music_List_Name)::value_type>(_music_List_Name_value));}}, [&]() { return &_music_List_Name; }, [&]() { std::ostringstream oss;for (auto &value : _music_List_Name) {oss << value << ",";}return oss.str();}};
	metadata["_sound_List_Name"] = {"std::vector<std::string>", "_sound_List_Name", sizeof(std::vector<std::string>), [&](const std::string &str) {std::stringstream _sound_List_Name_ss(str);std::string _sound_List_Name_value;while (getline(_sound_List_Name_ss, _sound_List_Name_value, ',')) {_sound_List_Name.push_back(fromString<decltype(_sound_List_Name)::value_type>(_sound_List_Name_value));}}, [&]() { return &_sound_List_Name; }, [&]() { std::ostringstream oss;for (auto &value : _sound_List_Name) {oss << value << ",";}return oss.str();}};
	return metadata;
}
