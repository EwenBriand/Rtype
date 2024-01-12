/*
* CAUTION: This file was generated automatically.
* Do not modify it, as your changes will be lost.
*/

#include <sstream>
#include "ToBytes.hpp"
#include <iostream>
#include "metadata.hpp"
#include "Boss.hpp"

void Boss::Load(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Load(filepath, _speed, _hp, _bodyParts);
}

void Boss::Save(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Save(filepath, _speed, _hp, _bodyParts);
}

std::map<std::string, metadata_t> Boss::GetMetadata()
{
	std::map<std::string, metadata_t> metadata;
	metadata["_speed"] = {"float", "_speed", sizeof(float), [&](const std::string &str) {_speed = fromString<decltype(_speed)>(str);}, [&]() { return &_speed; }, [&]() { std::ostringstream oss;oss << _speed;return oss.str();}};
	metadata["_hp"] = {"float", "_hp", sizeof(float), [&](const std::string &str) {_hp = fromString<decltype(_hp)>(str);}, [&]() { return &_hp; }, [&]() { std::ostringstream oss;oss << _hp;return oss.str();}};
	metadata["_bodyParts"] = {"int", "_bodyParts", sizeof(int), [&](const std::string &str) {_bodyParts = fromString<decltype(_bodyParts)>(str);}, [&]() { return &_bodyParts; }, [&]() { std::ostringstream oss;oss << _bodyParts;return oss.str();}};
	return metadata;
}
