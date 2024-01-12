/*
* CAUTION: This file was generated automatically.
* Do not modify it, as your changes will be lost.
*/

#include <sstream>
#include "ToBytes.hpp"
#include <iostream>
#include "metadata.hpp"
#include "RigidBody3D.hpp"

void RigidBody3D::Load(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Load(filepath, _dragFactor, _mass, _isKinematic, _simulateGravity, _lockX, _lockY, _lockZ, _lockRotX, _lockRotY, _lockRotZ, _elasticity);
}

void RigidBody3D::Save(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Save(filepath, _dragFactor, _mass, _isKinematic, _simulateGravity, _lockX, _lockY, _lockZ, _lockRotX, _lockRotY, _lockRotZ, _elasticity);
}

std::map<std::string, metadata_t> RigidBody3D::GetMetadata()
{
	std::map<std::string, metadata_t> metadata;
	metadata["_dragFactor"] = {"float", "_dragFactor", sizeof(float), [&](const std::string &str) {_dragFactor = fromString<decltype(_dragFactor)>(str);}, [&]() { return &_dragFactor; }, [&]() { std::ostringstream oss;oss << _dragFactor;return oss.str();}};
	metadata["_mass"] = {"int", "_mass", sizeof(int), [&](const std::string &str) {_mass = fromString<decltype(_mass)>(str);}, [&]() { return &_mass; }, [&]() { std::ostringstream oss;oss << _mass;return oss.str();}};
	metadata["_isKinematic"] = {"bool", "_isKinematic", sizeof(bool), [&](const std::string &str) {_isKinematic = fromString<decltype(_isKinematic)>(str);}, [&]() { return &_isKinematic; }, [&]() { std::ostringstream oss;oss << _isKinematic;return oss.str();}};
	metadata["_simulateGravity"] = {"bool", "_simulateGravity", sizeof(bool), [&](const std::string &str) {_simulateGravity = fromString<decltype(_simulateGravity)>(str);}, [&]() { return &_simulateGravity; }, [&]() { std::ostringstream oss;oss << _simulateGravity;return oss.str();}};
	metadata["_lockX"] = {"bool", "_lockX", sizeof(bool), [&](const std::string &str) {_lockX = fromString<decltype(_lockX)>(str);}, [&]() { return &_lockX; }, [&]() { std::ostringstream oss;oss << _lockX;return oss.str();}};
	metadata["_lockY"] = {"bool", "_lockY", sizeof(bool), [&](const std::string &str) {_lockY = fromString<decltype(_lockY)>(str);}, [&]() { return &_lockY; }, [&]() { std::ostringstream oss;oss << _lockY;return oss.str();}};
	metadata["_lockZ"] = {"bool", "_lockZ", sizeof(bool), [&](const std::string &str) {_lockZ = fromString<decltype(_lockZ)>(str);}, [&]() { return &_lockZ; }, [&]() { std::ostringstream oss;oss << _lockZ;return oss.str();}};
	metadata["_lockRotX"] = {"float", "_lockRotX", sizeof(float), [&](const std::string &str) {_lockRotX = fromString<decltype(_lockRotX)>(str);}, [&]() { return &_lockRotX; }, [&]() { std::ostringstream oss;oss << _lockRotX;return oss.str();}};
	metadata["_lockRotY"] = {"float", "_lockRotY", sizeof(float), [&](const std::string &str) {_lockRotY = fromString<decltype(_lockRotY)>(str);}, [&]() { return &_lockRotY; }, [&]() { std::ostringstream oss;oss << _lockRotY;return oss.str();}};
	metadata["_lockRotZ"] = {"float", "_lockRotZ", sizeof(float), [&](const std::string &str) {_lockRotZ = fromString<decltype(_lockRotZ)>(str);}, [&]() { return &_lockRotZ; }, [&]() { std::ostringstream oss;oss << _lockRotZ;return oss.str();}};
	metadata["_elasticity"] = {"float", "_elasticity", sizeof(float), [&](const std::string &str) {_elasticity = fromString<decltype(_elasticity)>(str);}, [&]() { return &_elasticity; }, [&]() { std::ostringstream oss;oss << _elasticity;return oss.str();}};
	return metadata;
}
