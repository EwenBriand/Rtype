/*
* CAUTION: This file was generated automatically.
* Do not modify it, as your changes will be lost.
*/

#include <sstream>
#include "ToBytes.hpp"
#include <iostream>
#include "metadata.hpp"
#include "Ship.hpp"

void Ship::Load(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Load(filepath, _speed, _damage, _health);
}

void Ship::Save(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Save(filepath, _speed, _damage, _health);
}

std::map<std::string, metadata_t> Ship::GetMetadata()
{
	std::map<std::string, metadata_t> metadata;
	metadata["_speed"] = {"float", "_speed", sizeof(float), [&](const std::string &str) {_speed = fromString<decltype(_speed)>(str);}, [&]() { return &_speed; }, [&]() { std::ostringstream oss;oss << _speed;return oss.str();}};
	metadata["_damage"] = {"int", "_damage", sizeof(int), [&](const std::string &str) {_damage = fromString<decltype(_damage)>(str);}, [&]() { return &_damage; }, [&]() { std::ostringstream oss;oss << _damage;return oss.str();}};
	metadata["_health"] = {"int", "_health", sizeof(int), [&](const std::string &str) {_health = fromString<decltype(_health)>(str);}, [&]() { return &_health; }, [&]() { std::ostringstream oss;oss << _health;return oss.str();}};
	return metadata;
}
