/*
* CAUTION: This file was generated automatically.
* Do not modify it, as your changes will be lost.
*/

#include <sstream>
#include "ToBytes.hpp"
#include <iostream>
#include "metadata.hpp"
#include "RLCamera3D.hpp"

void RLCamera3D::Load(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Load(filepath, m_px, m_py, m_pz, m_tx, m_ty, m_tz, m_ux, m_uy, m_uz, m_fov, m_projection);
}

void RLCamera3D::Save(const std::string &filepath)
{
	bytes::ToBytes bytes;
	bytes.Save(filepath, m_px, m_py, m_pz, m_tx, m_ty, m_tz, m_ux, m_uy, m_uz, m_fov, m_projection);
}

std::map<std::string, metadata_t> RLCamera3D::GetMetadata()
{
	std::map<std::string, metadata_t> metadata;
	metadata["m_px"] = {"float", "m_px", sizeof(float), [&](const std::string &str) {m_px = fromString<decltype(m_px)>(str);}, [&]() { return &m_px; }, [&]() { std::ostringstream oss;oss << m_px;return oss.str();}};
	metadata["m_py"] = {"float", "m_py", sizeof(float), [&](const std::string &str) {m_py = fromString<decltype(m_py)>(str);}, [&]() { return &m_py; }, [&]() { std::ostringstream oss;oss << m_py;return oss.str();}};
	metadata["m_pz"] = {"float", "m_pz", sizeof(float), [&](const std::string &str) {m_pz = fromString<decltype(m_pz)>(str);}, [&]() { return &m_pz; }, [&]() { std::ostringstream oss;oss << m_pz;return oss.str();}};
	metadata["m_tx"] = {"float", "m_tx", sizeof(float), [&](const std::string &str) {m_tx = fromString<decltype(m_tx)>(str);}, [&]() { return &m_tx; }, [&]() { std::ostringstream oss;oss << m_tx;return oss.str();}};
	metadata["m_ty"] = {"float", "m_ty", sizeof(float), [&](const std::string &str) {m_ty = fromString<decltype(m_ty)>(str);}, [&]() { return &m_ty; }, [&]() { std::ostringstream oss;oss << m_ty;return oss.str();}};
	metadata["m_tz"] = {"float", "m_tz", sizeof(float), [&](const std::string &str) {m_tz = fromString<decltype(m_tz)>(str);}, [&]() { return &m_tz; }, [&]() { std::ostringstream oss;oss << m_tz;return oss.str();}};
	metadata["m_ux"] = {"float", "m_ux", sizeof(float), [&](const std::string &str) {m_ux = fromString<decltype(m_ux)>(str);}, [&]() { return &m_ux; }, [&]() { std::ostringstream oss;oss << m_ux;return oss.str();}};
	metadata["m_uy"] = {"float", "m_uy", sizeof(float), [&](const std::string &str) {m_uy = fromString<decltype(m_uy)>(str);}, [&]() { return &m_uy; }, [&]() { std::ostringstream oss;oss << m_uy;return oss.str();}};
	metadata["m_uz"] = {"float", "m_uz", sizeof(float), [&](const std::string &str) {m_uz = fromString<decltype(m_uz)>(str);}, [&]() { return &m_uz; }, [&]() { std::ostringstream oss;oss << m_uz;return oss.str();}};
	metadata["m_fov"] = {"float", "m_fov", sizeof(float), [&](const std::string &str) {m_fov = fromString<decltype(m_fov)>(str);}, [&]() { return &m_fov; }, [&]() { std::ostringstream oss;oss << m_fov;return oss.str();}};
	metadata["m_projection"] = {"std::string", "m_projection", sizeof(std::string), [&](const std::string &str) {m_projection = fromString<decltype(m_projection)>(str);}, [&]() { return &m_projection; }, [&]() { std::ostringstream oss;oss << m_projection;return oss.str();}};
	return metadata;
}
