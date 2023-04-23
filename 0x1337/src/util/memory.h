#pragma once
#include <cstdint>
#include <functional>

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
}