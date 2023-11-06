/*
* CAUTION: This file was generated automatically.
* Do not modify it, as your changes will be lost.
*/

#include "header.hpp"
#include "metadata.hpp"

void Test::Load(libconfig::Setting &setting)
{
	setting.lookupValue("testInt", testInt);
	setting.lookupValue("testString", testString);
}

void Test::Save(const std::string &filepath)
{
	libconfig::Config config;
	libconfig::Setting &root = config.getRoot();
	root.add("testInt", testInt);
	root.add("testString", testString);
	config.writeFile(filepath.c_str());
}

std::map<std::string, metadata_t> Test::GetMetadata()
{
	std::map<std::string, metadata_t> metadata;
	metadata["testInt"] = {"int", "testInt", offsetof(Test, testInt), sizeof(int)};
	metadata["testString"] = {"std::string", "testString", offsetof(Test, testString), sizeof(std::string)};
	return metadata;
}
