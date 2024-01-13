/*
* CAUTION: This file was generated automatically.
* Do not modify it, as your changes will be lost.
*/

#include <sstream>
#include "ToBytes.hpp"
#include <iostream>
#include "metadata.hpp"
#include "RigidBody2D.hpp"

void RigidBody2D::Load(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Load(filepath, _simulateGravity, _isKinematic, _mass, _elasticity);
}

void RigidBody2D::Save(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Save(filepath, _simulateGravity, _isKinematic, _mass, _elasticity);
}

std::map<std::string, metadata_t> RigidBody2D::GetMetadata()
{
	std::map<std::string, metadata_t> metadata;
	metadata["_simulateGravity"] = {"int", "_simulateGravity", sizeof(int), [&](const std::string &str) {_simulateGravity = fromString<decltype(_simulateGravity)>(str);}, [&]() { return &_simulateGravity; }, [&]() { std::ostringstream oss;oss << _simulateGravity;return oss.str();}};
	metadata["_isKinematic"] = {"int", "_isKinematic", sizeof(int), [&](const std::string &str) {_isKinematic = fromString<decltype(_isKinematic)>(str);}, [&]() { return &_isKinematic; }, [&]() { std::ostringstream oss;oss << _isKinematic;return oss.str();}};
	metadata["_mass"] = {"float", "_mass", sizeof(float), [&](const std::string &str) {_mass = fromString<decltype(_mass)>(str);}, [&]() { return &_mass; }, [&]() { std::ostringstream oss;oss << _mass;return oss.str();}};
	metadata["_elasticity"] = {"float", "_elasticity", sizeof(float), [&](const std::string &str) {_elasticity = fromString<decltype(_elasticity)>(str);}, [&]() { return &_elasticity; }, [&]() { std::ostringstream oss;oss << _elasticity;return oss.str();}};
	return metadata;
}
