/*
* CAUTION: This file was generated automatically.
* Do not modify it, as your changes will be lost.
*/

#include <sstream>
#include "ToBytes.hpp"
#include <iostream>
#include "metadata.hpp"
#include "NodeStr.hpp"

void NodeStr::Load(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Load(filepath, output_text);
}

void NodeStr::Save(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Save(filepath, output_text);
}

std::map<std::string, metadata_t> NodeStr::GetMetadata()
{
	std::map<std::string, metadata_t> metadata;
	metadata["output_text"] = {"std::string", "output_text", sizeof(std::string), [&](const std::string &str) {output_text = fromString<decltype(output_text)>(str);}, [&]() { return &output_text; }, [&]() { std::ostringstream oss;oss << output_text;return oss.str();}};
	return metadata;
}
