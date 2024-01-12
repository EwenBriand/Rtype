/*
* CAUTION: This file was generated automatically.
* Do not modify it, as your changes will be lost.
*/

#include <sstream>
#include "ToBytes.hpp"
#include <iostream>
#include "metadata.hpp"
#include "Animator.hpp"

void Animator::Load(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Load(filepath, _currentAnimation, _registeredAnimations);
}

void Animator::Save(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Save(filepath, _currentAnimation, _registeredAnimations);
}

std::map<std::string, metadata_t> Animator::GetMetadata()
{
	std::map<std::string, metadata_t> metadata;
	metadata["_currentAnimation"] = {"std::string", "_currentAnimation", sizeof(std::string), [&](const std::string &str) {_currentAnimation = fromString<decltype(_currentAnimation)>(str);}, [&]() { return &_currentAnimation; }, [&]() { std::ostringstream oss;oss << _currentAnimation;return oss.str();}};
	metadata["_registeredAnimations"] = {"std::vector<std::string>", "_registeredAnimations", sizeof(std::vector<std::string>), [&](const std::string &str) {std::stringstream _registeredAnimations_ss(str);std::string _registeredAnimations_value;while (getline(_registeredAnimations_ss, _registeredAnimations_value, ',')) {_registeredAnimations.push_back(fromString<decltype(_registeredAnimations)::value_type>(_registeredAnimations_value));}}, [&]() { return &_registeredAnimations; }, [&]() { std::ostringstream oss;for (auto &value : _registeredAnimations) {oss << value << ",";}return oss.str();}};
	return metadata;
}
