#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <thread>

#include "core/hooks.h"

DWORD WINAPI Setup(LPVOID instance)
{
	interfaces::Setup();
	hooks::Setup();

	while (!GetAsyncKeyState(VK_DELETE))
		std::this_thread::sleep_for(std::chrono::milliseconds(200));

	hooks::Destroy();
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
	}

	return true;
}