#include "hooks.h"
#include <windows.h>

DWORD WINAPI InitHooks(LPVOID) {
    SetupHooks();
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD  reason,
                      LPVOID reserved)
{
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        MessageBoxA(NULL, "DLL Injected", "Hook Test", MB_OK);

        // Run hooks in a separate thread (important!)
        CreateThread(NULL, 0, InitHooks, NULL, 0, NULL);
    }
    return TRUE;
}
