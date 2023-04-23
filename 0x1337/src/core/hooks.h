#pragma once
#include "interfaces.h"
//#include "../util/vfunc_hook.hpp"


#include <d3d9.h>

namespace index
{

	//constexpr auto CreateMove = 24;
	//constexpr auto FrameStageNotify = 37;
	//constexpr auto DrawModelExecute = 21;
	//constexpr auto AllocKeyValuesMemory = 1;
}

namespace hooks {
	void Setup();
	void Destroy() noexcept;

	// EndScene hook for directx 9 hooking
	using EndSceneFn = long(__thiscall*)(void*, IDirect3DDevice9*) noexcept;
	inline EndSceneFn EndSceneOriginal = nullptr;

	using ResetFn = HRESULT(__thiscall*)(void*, IDirect3DDevice9*, D3DPRESENT_PARAMETERS*) noexcept;
	inline ResetFn ResetOriginal = nullptr;

	// for in game cursor locking
	using LockCursorFn = void(__thiscall*)(void*) noexcept;
	inline LockCursorFn LockCursorOriginal = nullptr;
	void __stdcall hkLockCursor() noexcept;
	
}