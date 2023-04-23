#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <thread>

#include "core/hooks.h"
#include "core/gui.h"

//namespace gui {
//	void Setup();
//	void Destroy();
//}

DWORD WINAPI Setup(LPVOID instance)
{
	try {
		interfaces::Setup();
		//memory::Setup();
		//netvars::Setup();
		gui::Setup();
		hooks::Setup();
	}
	catch (const std::exception& error){
		MessageBeep(MB_ICONERROR);
		MessageBox(
			0,
			error.what(),
			"error loading GameRecord process",
			MB_OK | MB_ICONEXCLAMATION
		);

		globals::shouldUnload = true;
	}

	while (/*!GetAsyncKeyState(VK_DELETE) && */ !globals::shouldUnload)
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	

	gui::open = false;
	interfaces::inputSystem->EnableInput(true);

	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	hooks::Destroy();

	std::this_thread::sleep_for(std::chrono::milliseconds(100));

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