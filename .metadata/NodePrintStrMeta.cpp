/*
* CAUTION: This file was generated automatically.
* Do not modify it, as your changes will be lost.
*/

#include <sstream>
#include "ToBytes.hpp"
#include <iostream>
#include "metadata.hpp"
#include "NodePrintStr.hpp"

void NodePrintStr::Load(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Load(filepath, input_text);
}

void NodePrintStr::Save(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Save(filepath, input_text);
}

std::map<std::string, metadata_t> NodePrintStr::GetMetadata()
{
	std::map<std::string, metadata_t> metadata;
	metadata["input_text"] = {"std::string", "input_text", sizeof(std::string), [&](const std::string &str) {input_text = fromString<decltype(input_text)>(str);}, [&]() { return &input_text; }, [&]() { std::ostringstream oss;oss << input_text;return oss.str();}};
	return metadata;
}
