#pragma once
#include "../util/memory.h"

#include "cvector.h"
#include "cusercmd.h"

class IEngineClient
{
public:
	// returns index of local players
	constexpr std::int32_t GetLocalPlayerIndex() noexcept
	{
		return memory::Call<std::int32_t>(this, 12);
	}

	// sets client view angles
	constexpr void SetViewAngles(const CVector& viewAngles) noexcept
	{
		memory::Call<void, const CVector&>(this, 19, viewAngles);
	}
};