#pragma once

class CEntity;
namespace globals {
	// global local player pointer (Inline so it doesn't get re defined ewvery time we include this file)
	inline CEntity* localPlayer = nullptr;
}