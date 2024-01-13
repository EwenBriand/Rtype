/*
* CAUTION: This file was generated automatically.
* Do not modify it, as your changes will be lost.
*/

#include <sstream>
#include "ToBytes.hpp"
#include <iostream>
#include "metadata.hpp"
#include "SplashIcon.hpp"

void SplashIcon::Load(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Load(filepath, _iconPath, _rotationSpeed);
}

void SplashIcon::Save(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Save(filepath, _iconPath, _rotationSpeed);
}

std::map<std::string, metadata_t> SplashIcon::GetMetadata()
{
	std::map<std::string, metadata_t> metadata;
	metadata["_iconPath"] = {"std::string", "_iconPath", sizeof(std::string), [&](const std::string &str) {_iconPath = fromString<decltype(_iconPath)>(str);}, [&]() { return &_iconPath; }, [&]() { std::ostringstream oss;oss << _iconPath;return oss.str();}};
	metadata["_rotationSpeed"] = {"float", "_rotationSpeed", sizeof(float), [&](const std::string &str) {_rotationSpeed = fromString<decltype(_rotationSpeed)>(str);}, [&]() { return &_rotationSpeed; }, [&]() { std::ostringstream oss;oss << _rotationSpeed;return oss.str();}};
	return metadata;
}
