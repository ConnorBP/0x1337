#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <thread>

#include "core/hooks.h"

namespace gui {
	void Setup();
	void Destroy();
}

DWORD WINAPI Setup(LPVOID instance)
{
	try {
		interfaces::Setup();
		memory::Setup();
		netvars::Setup();
		gui::Setup();
		hooks::Setup();
	}
	catch (const std::exception& error){
		MessageBeep(MB_ICONERROR);
		MessageBox(
			0,
			error.what(),
			"0x1337 error",
			MB_OK | MB_ICONEXCLAMATION
		);

		globals::shouldUnload = true;
	}

	//x86RetSpoof::invokeStdcall<int>(
	//	std::uintptr_t(&MessageBoxW),
	//	std::uintptr_t(memory::clientGadgetAddress),
	//	nullptr,
	//	L"return address spoof working",
	//	L"Success",
	//	MB_OK
	//);


	while (!GetAsyncKeyState(VK_DELETE) && !globals::shouldUnload)
		std::this_thread::sleep_for(std::chrono::milliseconds(200));

	hooks::Destroy();
	gui::Destroy();
	FreeLibraryAndExitThread(static_cast<HMODULE>(instance), EXIT_SUCCESS);
}

BOOL APIENTRY DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		// disable thread notifications
		DisableThreadLibraryCalls(instance);

		// create setup thread
		const HANDLE thread = CreateThread(
			nullptr,
			0,
			Setup,
			instance,
			0,
			nullptr
		);
		if (thread)
			CloseHandle(thread);
		// beep to alert injuection
		MessageBeep(MB_OK);
	}

	return true;
}