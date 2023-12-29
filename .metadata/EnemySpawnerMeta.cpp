/*
* CAUTION: This file was generated automatically.
* Do not modify it, as your changes will be lost.
*/

#include <sstream>
#include "ToBytes.hpp"
#include <iostream>
#include "metadata.hpp"
#include "EnemySpawner.hpp"

void EnemySpawner::Load(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Load(filepath, _spawnDelay, _prefabName);
}

void EnemySpawner::Save(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Save(filepath, _spawnDelay, _prefabName);
}

std::map<std::string, metadata_t> EnemySpawner::GetMetadata()
{
	std::map<std::string, metadata_t> metadata;
	metadata["_spawnDelay"] = {"float", "_spawnDelay", sizeof(float), [&](const std::string &str) {_spawnDelay = fromString<decltype(_spawnDelay)>(str);}, [&]() { return &_spawnDelay; }, [&]() { std::ostringstream oss;oss << _spawnDelay;return oss.str();}};
	metadata["_prefabName"] = {"std::string", "_prefabName", sizeof(std::string), [&](const std::string &str) {_prefabName = fromString<decltype(_prefabName)>(str);}, [&]() { return &_prefabName; }, [&]() { std::ostringstream oss;oss << _prefabName;return oss.str();}};
	return metadata;
}
