#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <d3d9.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <stdexcept>
#include <intrin.h>

#include "globals.h"

struct nk_context;
struct nk_colorf;

namespace gui
{
	// show or hide the menu
	inline bool open = true;
	// to run setup only once
	inline bool doneSetup = false;

	// winapi stuff
	inline HWND window = nullptr;
	inline WNDCLASSEX windowClass = {};
	inline WNDPROC originalWindowProcess = nullptr;

	// dx stuff
	//inline LPDIRECT3DDEVICE9 device = nullptr;
	//inline LPDIRECT3DDEVICE9EX deviceEx = nullptr;
	inline LPDIRECT3D9  d3d9 = nullptr;
	inline  D3DPRESENT_PARAMETERS presentParams;

	// nk stuff
	inline struct nk_context* ctx;

	bool SetupWindowClass(const char* windowClassName) noexcept;
	void DestroyWindowClass() noexcept;

	bool SetupWindow(const char* windowName) noexcept;
	void DestroyWindow() noexcept;

	bool SetupDirectX() noexcept;
	void DestroyDirectX() noexcept;

	// setup device
	void Setup();

	void SetupMenu(LPDIRECT3DDEVICE9 device) noexcept;
	void Destroy() noexcept;

	void Render() noexcept;
}

namespace hooks {
	long __stdcall EndScene(IDirect3DDevice9* device) noexcept;
	HRESULT __stdcall Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept;
}