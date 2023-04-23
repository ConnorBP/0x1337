#pragma once

#include <d3d9.h>

class CEntity;
namespace globals {

	// if this is set to true, the program will unload
	inline bool shouldUnload = false;

}

namespace gui {
	// dx stuff
	inline LPDIRECT3DDEVICE9 device = nullptr;
}