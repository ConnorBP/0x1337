#pragma once
#include "globals.h"

#include "../valve/ibaseclientdll.h"
#include "../valve/icliententitylist.h"
#include "../valve/iengineclient.h"
#include "../valve/ienginetraceclient.h"
#include "../valve/imaterialsystem.h"
#include "../valve/istudiorender.h"

namespace interfaces
{
	// call once to set up interfaces
	void Setup() noexcept;

	// capture interfaces via version strings
	// using exported CreateInterface function
	template <typename T>
	T* Capture(const char* name, const char* lib) noexcept;

	inline IClientEntityList* entityList = nullptr;
	inline IEngineClient* engine = nullptr;
	inline IEngineTraceClient* trace = nullptr;
	inline IBaseClientDLL* client = nullptr;
	inline void* clientMode = nullptr;
	inline IMaterialSystem* materialSystem = nullptr;
	inline IStudioRender* studioRender = nullptr;

	// other
	inline void* keyValuesSystem = nullptr;

};