#pragma once
#include "globals.h"

#include "../valve/i_input_system.hpp"
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

	inline i_inputsytem* inputSystem;
	inline ISurface* surface;



};