#pragma once
#include <winsock2.h>
#include <windows.h>

// Original function pointers
extern LPVOID (WINAPI *RealVirtualAlloc)(LPVOID, SIZE_T, DWORD, DWORD);
extern FARPROC (WINAPI *RealGetProcAddress)(HMODULE, LPCSTR);
extern int (WSAAPI *RealSend)(SOCKET, const char*, int, int);

// Hooked versions
LPVOID WINAPI HookedVirtualAlloc(LPVOID, SIZE_T, DWORD, DWORD);
FARPROC WINAPI HookedGetProcAddress(HMODULE, LPCSTR);
int WSAAPI HookedSend(SOCKET, const char*, int, int);

void SetupHooks();
