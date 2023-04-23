#include "hooks.h"

// include minhook for epic hookage
#include "../../ext/minhook/minhook.h"

#include <stdexcept>
#include <intrin.h>

#include "config.h"

#include "gui.h"

#include "../util/xorstr.hpp"

void hooks::Setup()
{
	// minhook returns non zero on error
	if (MH_Initialize())
		throw std::runtime_error(xorstr_("Unable to initialize minhook"));

	//endscene hook
	if (MH_CreateHook(
		memory::Get(gui::device, 16),
		&Reset,
		reinterpret_cast<void**>(&ResetOriginal)
	))
		throw std::runtime_error(xorstr_("failed to create DeviceReset hook"));

	//reset hook
	if (MH_CreateHook(
		memory::Get(gui::device, 42),
		&EndScene,
		reinterpret_cast<void**>(&EndSceneOriginal)
	))
		throw std::runtime_error(xorstr_("failed to create endscene hook"));

	////reset hook
	if (MH_CreateHook(
		memory::Get(interfaces::surface, 67),
		&hkLockCursor,
		reinterpret_cast<void**>(&LockCursorOriginal)
	))
		throw std::runtime_error(xorstr_("failed to create hkLockCursor hook"));

	if (MH_EnableHook(MH_ALL_HOOKS))
		throw std::runtime_error(xorstr_("unable to enable hooks"));

	// we don't need this once we use it to create the hooks
	gui::DestroyDirectX();
}

void hooks::Destroy() noexcept
{
	// restore hooks
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);

	// uninit minhook
	MH_Uninitialize();
}

void __stdcall hooks::hkLockCursor() noexcept
{
	if (gui::open)
		return interfaces::surface->UnlockCursor();
	return LockCursorOriginal(interfaces::surface);
}