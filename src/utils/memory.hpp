#pragma once

#include <cstdint>
#include <functional>
#include <string>

namespace memory {
	void Setup() noexcept;

	template<typename Return, typename ... Arguments>
	constexpr Return Call(void* vmt, std::uint32_t index, Arguments ... args) noexcept {
		using Function = Return(__thiscall*)(void*, decltype(args)...);
		return (*static_cast<Function**>(vmt))[index](vmt, args...);
	}


	template<typename type>
	constexpr type Get(void* clazz, const std::uint32_t offset) {
		return *reinterpret_cast<type*>(uintptr_t(clazz) + offset);
	}


	constexpr void* Get(void* vmt, const std::uint32_t index) {
		return (*static_cast<void***>(vmt))[index];
	}

	inline std::uintptr_t RelativeToAbsolute(std::uintptr_t relAddr) noexcept {
		return static_cast<std::uintptr_t>(relAddr + 4 + *reinterpret_cast<std::int32_t*>(relAddr));
	}

	unsigned long PatternSearch(std::string moduleName, std::string Pattern, std::string mask, unsigned long uCodeBase, unsigned long uSizeOfCode) noexcept;

	std::uint8_t* PatternScan(const char* moduleName, const char* pattern) noexcept;
	std::uint8_t* PatternScan(void* module, const char* signature) noexcept;
	inline std::uint8_t* allocKeyValuesClient = nullptr;
	inline std::uint8_t* allocKeyValuesEngine = nullptr;
	inline std::uint8_t* insertIntoTree = nullptr;

	inline std::uint8_t* keyValuesFromString = nullptr;
}