#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

DWORD FindProcess(const char* name) {
    PROCESSENTRY32 pe = { sizeof(pe) };
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    Process32First(snap, &pe);

    while (Process32Next(snap, &pe)) {
        if (strcmp(pe.szExeFile, name) == 0) {
            return pe.th32ProcessID;
        }
    }
    return 0;
}

int main() {
    DWORD pid = FindProcess("malware.exe");

    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProc) {
        std::cout << "OpenProcess failed\n";
        return 1;
    }

    void* mem = VirtualAllocEx(hProc, NULL, 260,
                              MEM_COMMIT | MEM_RESERVE,
                              PAGE_READWRITE);

    const char* dllPath = "D:\\CSCE652\\neuter_trickbot\\defense.dll";
    size_t pathLen = strlen(dllPath) + 1; 

    BOOL success = WriteProcessMemory(hProc, mem, dllPath, pathLen, NULL);

    if (!success) {
        printf("WriteProcessMemory failed. Error: %lu\n", GetLastError());
    }

    CreateRemoteThread(hProc, NULL, 0,
        (LPTHREAD_START_ROUTINE)LoadLibraryA,
        mem, 0, NULL);

    std::cout << "Injected.\n";
#if 0
    if (pid != 0) {
        HANDLE hProc = OpenProcess(PROCESS_TERMINATE, FALSE, pid);

        if (hProc) {
            TerminateProcess(hProc, 1);
            CloseHandle(hProc);
            std::cout << "Killed malware\n";
        }
    }
#endif
    Sleep(50000);
}
