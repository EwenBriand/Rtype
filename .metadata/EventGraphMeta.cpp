/*
* CAUTION: This file was generated automatically.
* Do not modify it, as your changes will be lost.
*/

#include <sstream>
#include "ToBytes.hpp"
#include <iostream>
#include "metadata.hpp"
#include "EventGraph.hpp"

void EventGraph::Load(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Load(filepath);
}

void EventGraph::Save(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Save(filepath);
}

std::map<std::string, metadata_t> EventGraph::GetMetadata()
{
	std::map<std::string, metadata_t> metadata;
	return metadata;
}
