#pragma once
#include "../util/memory.h"

#include "cvector.h"
#include "cusercmd.h"
#include "NetworkChannel.h"

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
	constexpr bool IsInGame() noexcept {
		return memory::Call<bool>(this, 26);
	}

	//VIRTUAL_METHOD(bool, isConnected, 27, (), (this))

	//VIRTUAL_METHOD(const char*, getLevelName, 53, (), (this))

	//VIRTUAL_METHOD(NetworkChannel*, getNetworkChannel, 78, (), (this))

	constexpr NetworkChannel* getNetworkChannel() noexcept {
		return memory::Call<NetworkChannel*>(this, 78);
	}

	constexpr void clientCmdUnrestricted(const char* cmd, bool fromConsoleOrKeybind = false) noexcept {
		return memory::Call<void>(this, 114, cmd, fromConsoleOrKeybind);
	}
};