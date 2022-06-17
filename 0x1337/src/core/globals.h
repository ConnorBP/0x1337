#pragma once

#include <d3d9.h>

class CEntity;
namespace globals {

	// if this is set to true, the cheat will unload
	inline bool shouldUnload = false;

	// global local player pointer (Inline so it doesn't get re defined ewvery time we include this file)
	inline CEntity* localPlayer = nullptr;
}

namespace gui {
	// dx stuff
	inline LPDIRECT3DDEVICE9 device = nullptr;
}