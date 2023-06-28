#pragma once
#include <format>
#include <ctype.h>
#include "../utils/datatable.hpp"
#include "../utils/hash.hpp"
#include <unordered_map>

#include <string>
#include <fstream>
#include <iostream>

namespace netvars {
	void SetupNetvars();
	void DumpClasses();
	const char* szConvertType(SendPropType type);

	void Dump(const char* baseClass, RecvTable* table, std::uint32_t offset = 0);

	std::uint32_t GetOffset(const char* baseclass, const char* propName);

	inline std::unordered_map<std::uint32_t, std::uint32_t> data = { };
}

#define NETVAR(name, var, ...) \
inline std::add_lvalue_reference_t<__VA_ARGS__> name() noexcept \
{ \
	static const std::uint32_t offset = netvars::data[fnv::CompileTime(var)]; \
	return *reinterpret_cast<std::add_pointer_t<__VA_ARGS__>>(std::uint32_t(this) + offset); \
}

#define OFFSET(name, baseclass, propname, type) \
	type name = memory::Get<type>(this, netvars::GetOffset(baseclass, propname));

#define RETURN_OFFSET(name, baseclass, propname, type) \
	memory::Get<type>(this, netvars::GetOffset(baseclass, propname));