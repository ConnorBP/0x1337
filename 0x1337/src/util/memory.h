#pragma once
#include <cstdint>
#include <functional>

#include "x86RetSpoof.h"

namespace memory
{
	// call once to scan for all patterns
	void Setup() noexcept;

	// given an interface pointer, calls the function
	// at the index provided
	template <typename Return, typename ... Args>
	constexpr Return Call(void* _this, const std::uintptr_t index, Args ... args) noexcept
	{
		using Function = Return(__thiscall*)(void*, decltype(args)...);
		return (*static_cast<Function**>(_this))[index](_this, args...);
	}

	constexpr void* Get(void* _this, const std::uintptr_t index) noexcept
	{
		return (*static_cast<void***>(_this))[index];
	}

	inline std::uintptr_t RelativeToAbsolute(std::uintptr_t relAddr) noexcept {
		return static_cast<std::uintptr_t>(relAddr + 4 + *reinterpret_cast<std::int32_t*>(relAddr));
	}

	// simple Pattern/AOB/Signature scanner
	std::uint8_t* PatternScan(const char* moduleName, const char* pattern) noexcept;

	// return addresses
	inline std::uint8_t* allocKeyValuesClient = nullptr;
	inline std::uint8_t* allocKeyValuesEngine = nullptr;
	inline std::uint8_t* insertIntoTree = nullptr;

	inline std::uint8_t* clientReturnCheck = nullptr;
	inline std::uint8_t* engineReturnCheck = nullptr;
	inline std::uint8_t* studiorenderReturnCheck = nullptr;
	inline std::uint8_t* materialsystemReturnCheck = nullptr;

	// x86RetSpoof gadget addresses
	inline std::uint8_t* clientGadgetAddress = nullptr;

	// functions
	inline std::uint8_t* keyValuesFromString = nullptr;

	// interfaces
	inline std::uint8_t* glowManager = nullptr;

}