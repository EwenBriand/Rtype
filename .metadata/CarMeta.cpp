/*
* CAUTION: This file was generated automatically.
* Do not modify it, as your changes will be lost.
*/

#include <sstream>
#include "ToBytes.hpp"
#include <iostream>
#include "metadata.hpp"
#include "Car.hpp"

void Car::Load(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Load(filepath, _maxSpeed, _acceleration, _deceleration, _rotationSpeed);
}

void Car::Save(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Save(filepath, _maxSpeed, _acceleration, _deceleration, _rotationSpeed);
}

std::map<std::string, metadata_t> Car::GetMetadata()
{
	std::map<std::string, metadata_t> metadata;
	metadata["_maxSpeed"] = {"float", "_maxSpeed", sizeof(float), [&](const std::string &str) {_maxSpeed = fromString<decltype(_maxSpeed)>(str);}, [&]() { return &_maxSpeed; }, [&]() { std::ostringstream oss;oss << _maxSpeed;return oss.str();}};
	metadata["_acceleration"] = {"float", "_acceleration", sizeof(float), [&](const std::string &str) {_acceleration = fromString<decltype(_acceleration)>(str);}, [&]() { return &_acceleration; }, [&]() { std::ostringstream oss;oss << _acceleration;return oss.str();}};
	metadata["_deceleration"] = {"float", "_deceleration", sizeof(float), [&](const std::string &str) {_deceleration = fromString<decltype(_deceleration)>(str);}, [&]() { return &_deceleration; }, [&]() { std::ostringstream oss;oss << _deceleration;return oss.str();}};
	metadata["_rotationSpeed"] = {"float", "_rotationSpeed", sizeof(float), [&](const std::string &str) {_rotationSpeed = fromString<decltype(_rotationSpeed)>(str);}, [&]() { return &_rotationSpeed; }, [&]() { std::ostringstream oss;oss << _rotationSpeed;return oss.str();}};
	return metadata;
}
