#pragma once
#include "interfaces.h"
//#include "../util/vfunc_hook.hpp"


#include <d3d9.h>

namespace index
{

	constexpr auto CreateMove = 24;
	constexpr auto FrameStageNotify = 37;
	constexpr auto DrawModelExecute = 21;
	constexpr auto AllocKeyValuesMemory = 1;
}

namespace hooks {
	void Setup();
	void Destroy() noexcept;

	//inline vfunc_hook clientmode_hook;

	
	// potentially bogus return address check bypass

	using VerifyReturnAddressFn = bool(__fastcall*)(void*, void*, const char*) noexcept;

	inline VerifyReturnAddressFn VerifyClientReturnOriginal = nullptr;
	inline VerifyReturnAddressFn VerifyEngineReturnOriginal = nullptr;
	inline VerifyReturnAddressFn VerifyStudioReturnOriginal = nullptr;
	inline VerifyReturnAddressFn VerifyMaterialReturnOriginal = nullptr;

	bool __fastcall verify_return_address_hook(void* ecx, void* edx, const char* module_name) noexcept;

	// bypass return address checks (thx osiris)
	using AllocKeyValuesMemoryFn = void* (__thiscall*)(void*, const std::int32_t) noexcept;
	inline AllocKeyValuesMemoryFn AllocKeyValuesMemoryOriginal;
	void* __stdcall AllocKeyValuesMemory(const std::int32_t size) noexcept;

	// createmove hook
	using CreateMoveFunction = void(__thiscall*)(void*, int, float, bool);
	inline CreateMoveFunction CreateMoveOriginal = nullptr;

	bool __stdcall CreateMove(float frameTime, CUserCmd* cmd, bool& sendPacket) noexcept;
	static void __stdcall CHLCreateMove(int sequenceNumber, float inputSampleTime, bool active, bool& sendPacket) noexcept;
	void __stdcall CreateMoveProxy(int sequenceNumber, float inputSampleTime, bool active);

	
	// chams hook

	// function signature
	using DrawModelFn = void(__thiscall*)(
		void*,
		void*,
		const CDrawModelInfo&,
		CMatrix3x4*,
		float*,
		float*,
		const CVector&,
		const std::int32_t
		) noexcept;

	// original function pointer
	inline DrawModelFn DrawModelOriginal = nullptr;

	// hooked function definition
	void __stdcall DrawModel(
		void* results,
		const CDrawModelInfo& info,
		CMatrix3x4* bones,
		float* flexWeights,
		float* flexDelayedWights,
		const CVector& modelOrigin,
		const std::int32_t flags
	) noexcept;

	// emit sound hook
	using EmitSoundFn = void(__thiscall*)(
		void*,
		IRecipientFilter&,
		int,
		int,
		const char*,
		unsigned int,
		const char*,
		float,
		int,
		float,
		int,
		int,
		const CVector*,
		const CVector*,
		void*,
		bool,
		float,
		int,
		int
		);

	inline EmitSoundFn EmitSoundOriginal = nullptr;
	//emit sound func definition
	void __stdcall hkEmitSound1(IRecipientFilter& filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample, float flVolume, int nSeed, float flAttenuation, int iFlags, int iPitch, const CVector* pOrigin, const CVector* pDirection, void* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, int unk);

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