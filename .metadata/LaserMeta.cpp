/*
* CAUTION: This file was generated automatically.
* Do not modify it, as your changes will be lost.
*/

#include <sstream>
#include "ToBytes.hpp"
#include <iostream>
#include "metadata.hpp"
#include "Laser.hpp"

void Laser::Load(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Load(filepath, _speed, _damage);
}

void Laser::Save(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Save(filepath, _speed, _damage);
}

std::map<std::string, metadata_t> Laser::GetMetadata()
{
	std::map<std::string, metadata_t> metadata;
	metadata["_speed"] = {"float", "_speed", sizeof(float), [&](const std::string &str) {_speed = fromString<decltype(_speed)>(str);}, [&]() { return &_speed; }, [&]() { std::ostringstream oss;oss << _speed;return oss.str();}};
	metadata["_damage"] = {"int", "_damage", sizeof(int), [&](const std::string &str) {_damage = fromString<decltype(_damage)>(str);}, [&]() { return &_damage; }, [&]() { std::ostringstream oss;oss << _damage;return oss.str();}};
	return metadata;
}
