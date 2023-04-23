#include "interfaces.h"

#define WIN32_LEAD_AND_MEAN
#include <Windows.h>

#include "../util/xorstr.hpp"

void interfaces::Setup() noexcept {
	inputSystem = Capture<i_inputsytem>(xorstr_("InputSystemVersion001"), xorstr_("inputsystem.dll"));
	surface = Capture<ISurface>(xorstr_("VGUI_Surface031"), xorstr_("vguimatsurface.dll"));
}

template <typename T>
T* interfaces::Capture(const char* name, const char* lib) noexcept
{
	const HINSTANCE handle = GetModuleHandle(lib);

	if (!handle)
		return nullptr;

	using Function = T * (*)(const char*, int*);
	Function CreateInterface = reinterpret_cast<Function>(GetProcAddress(handle, xorstr_("CreateInterface")));

	return CreateInterface(name, nullptr);
}