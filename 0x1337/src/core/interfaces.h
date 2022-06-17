#pragma once
#include "globals.h"

#include "../valve/ibaseclientdll.h"
#include "../valve/icliententitylist.h"
#include "../valve/iengineclient.h"
#include "../valve/ienginetraceclient.h"
#include "../valve/imaterialsystem.h"
#include "../valve/istudiorender.h"
#include "../valve/input.h"
#include "../valve/i_input_system.hpp"
#include "../valve/IEngineSound.hpp"
#include "../valve/isurface.h"

//forward declaration

//class Input;

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
	inline Input* input;
	inline i_inputsytem* inputSystem;
	inline IMaterialSystem* materialSystem = nullptr;
	inline IStudioRender* studioRender = nullptr;
	//inline IModelRender* modelRender = nullptr;
	inline ISurface* surface;

	// for auto accept
	inline IEngineSound* sound = nullptr;

	// other
	inline void* keyValuesSystem = nullptr;

};