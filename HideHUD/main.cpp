#include "f4se_common/Relocation.h"
#include "f4se_common/BranchTrampoline.h"
#include "SDL_defination.h"
#include <shlobj.h>

void		** g_settingsCollection = nullptr;

typedef		int (*_SDL_PollEvent)(_In_ SDL_Event * event);
_SDL_PollEvent	* PollEvent = nullptr;

UInt32		iHUDHotkey = SDL_SCANCODE_F11;

int SDL_PollEvent(SDL_Event * event)
{
	int result = (*PollEvent)(event);
	if (event != nullptr && event->type == SDL_KEYDOWN)
	{
		auto pKeyEvent = reinterpret_cast<SDL_KeyboardEvent*>(event);
		if (pKeyEvent->keysym.scancode == iHUDHotkey)
		{
			__try
			{
				if ((*g_settingsCollection) != nullptr)
				{
					bool & ui = *reinterpret_cast<bool*>((uintptr_t)(*g_settingsCollection) + 0x1A);
					ui = !ui;
				}
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{

			}
		}
	}
	return result;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\Larian Studios\\Divinity Original Sin 2\\NativeModLogs\\HideHUD.log");
		if (!g_branchTrampoline.Create(1024 * 64))
		{
			_ERROR("couldn't create branch trampoline. this is fatal. skipping remainder of init process.");
			return TRUE;
		}
		constexpr char * iniPath = ".\\NativeMods\\HideHUD.ini";
		constexpr char * sectionName = "Settings";
		iHUDHotkey = GetPrivateProfileIntA(sectionName, "iHUDHotkey", SDL_SCANCODE_F11, iniPath);

		try
		{
			uintptr_t location = RVAScan<uintptr_t>("48 83 EC 28 48 8B 15 ? ? ? ? B9 90 11 00 00 E8 ? ? ? ? 48 85 C0 74 14 48 8B C8 E8 ? ? ? ? 48 89 05 ? ? ? ? 48 83 C4 28 C3", 0x22, 3, 7).GetUIntPtr();
			_MESSAGE("(%016I64X) patched sucessfully...", location - RelocationManager::s_baseAddr);
			g_settingsCollection = reinterpret_cast<void **>(location);
		}
		catch (const no_result_exception & exceptioin)
		{
			_MESSAGE(exceptioin.what());
		}

		try
		{
			uintptr_t location = RVAScan<uintptr_t>("8B 45 70 3D 00 04 00 00 0F 87 67 05 00 00 0F 84 A2 03 00 00 2D 00 02 00 00 0F 84 F9 00 00 00", -0x88, 2, 6).GetUIntPtr();
			_MESSAGE("(%016I64X) patched sucessfully...", location - RelocationManager::s_baseAddr);
			PollEvent = reinterpret_cast<_SDL_PollEvent*>(location);
		}
		catch (const no_result_exception & exceptioin)
		{
			_MESSAGE(exceptioin.what());
		}

		try
		{
			uintptr_t location = RVAScan<uintptr_t>("8B 45 70 3D 00 04 00 00 0F 87 67 05 00 00 0F 84 A2 03 00 00 2D 00 02 00 00 0F 84 F9 00 00 00", -0x88).GetUIntPtr();
			_MESSAGE("(%016I64X) patched sucessfully...", location - RelocationManager::s_baseAddr);
			g_branchTrampoline.Write6Call(location, reinterpret_cast<uintptr_t>(SDL_PollEvent));
		}
		catch (const no_result_exception & exceptioin)
		{
			_MESSAGE(exceptioin.what());
		}

		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

