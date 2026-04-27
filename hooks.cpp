#include "hooks.h"
#include <windows.h>
#include <cstdio>
#include <iostream>
#include "MinHook.h"

void SetupHooks() {
    MH_Initialize();

    // Hook VirtualAlloc
    MH_CreateHook((LPVOID)VirtualAlloc,
		  (LPVOID)HookedVirtualAlloc,
		  (LPVOID*)&RealVirtualAlloc
		 );

    // Hook GetProcAddress
    MH_CreateHook(
    		  (LPVOID)GetProcAddress,
    		  (LPVOID)HookedGetProcAddress,
    		  (LPVOID*)&RealGetProcAddress
		);
    // Hook send (network exfiltration)
    MH_CreateHook(
    		  (LPVOID)send,
    		  (LPVOID)HookedSend,
    		  (LPVOID*)&RealSend
		);

    MH_EnableHook(MH_ALL_HOOKS);
}

LPVOID (WINAPI *RealVirtualAlloc)(LPVOID, SIZE_T, DWORD, DWORD);

LPVOID WINAPI HookedVirtualAlloc(LPVOID addr,
                                SIZE_T size,
                                DWORD allocType,
                                DWORD protect)
{
    if (protect & PAGE_EXECUTE_READWRITE ||
        protect & PAGE_EXECUTE_READ) {

        //OutputDebugStringA("[BLOCK] VirtualAlloc with EXECUTE\n");
	printf("[BLOCK] VirtualAlloc with EXECUTE\n");
        return NULL; // neuter malware
    }

    return RealVirtualAlloc(addr, size, allocType, protect);
}

FARPROC (WINAPI *RealGetProcAddress)(HMODULE, LPCSTR);

FARPROC WINAPI HookedGetProcAddress(HMODULE hModule, LPCSTR name)
{
    if ((ULONG_PTR)name > 0xFFFF) {
        //OutputDebugStringA("[API] Resolving: ");
        //OutputDebugStringA(name);
        //OutputDebugStringA("\n");
	printf("[API] Resolving: ");
	std::cout << name << std::endl;
    } else {
        //OutputDebugStringA("[API] Hashed/ordinal lookup\n");
	printf("[API] Hashed/ordinal lookup\n");
    }

    return RealGetProcAddress(hModule, name);
}

int (WSAAPI *RealSend)(SOCKET, const char*, int, int);

int WSAAPI HookedSend(SOCKET s,
                      const char* buf,
                      int len,
                      int flags)
{
    //OutputDebugStringA("[BLOCK] send() intercepted\n");
    printf("[BLOCK] send intercepted\n");
    char tmp[256] = {0};
    memcpy(tmp, buf, (len < 255 ? len : 255));

    //OutputDebugStringA("[DATA] ");
    //OutputDebugStringA(tmp);
    printf("[DATA] ");
    std::cout << tmp << std::endl;

    // simulate failure instead of success
    WSASetLastError(WSAEACCES);
    return SOCKET_ERROR;
}
