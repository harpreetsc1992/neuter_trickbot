# CSCE652: Trickbot Defense PoC (Hooking + Injection)

This project is a CSCE652 submission demonstrating a proof-of-concept defensive mechanism against Trickbot-like malware behavior.

The defense focuses on two primary malicious behaviors:

- Unauthorized virtual memory allocation targeting other processes
- Unauthorized network communication (data exfiltration via `send()`)

The system uses API hooking and process injection to intercept and neutralize these behaviors at runtime.

---

## Repository Setup

**Clone the repository:**

```bash
git clone git@github.com:harpreetsc1992/neuter_trickbot.git
# or
git clone https://github.com/harpreetsc1992/neuter_trickbot.git
cd neuter_trickbot
```
**Build Instructions:**

All components are compiled using the MinGW-w64 toolchain (x86\_64-w64-mingw32-g++) - install with ```bash sudo apt install mingw-w64 ```.
```bash
x86_64-w64-mingw32-g++ -shared -o defense.dll \
    dllmain.cpp hooks.cpp buffer.c trampoline.c hook.c hde/hde64.c \
    -lws2_32 -lpsapi -static
```
This generates defense.dll, which implements runtime API hooks for:

1. Memory allocation APIs (e.g., VirtualAlloc)
2. Network transmission APIs (e.g., send)

**Build the Test Malware (Controlled PoC Target):**
```bash
x86_64-w64-mingw32-g++ inject.cpp -o inject.exe \
    -lpsapi -static-libgcc -static-libstdc++
```
This program simulates malicious behavior:

1. Allocates memory using VirtualAlloc
2. Sends data over the network using send()

**Build the Injector:**
```bash
x86_64-w64-mingw32-g++ inject.cpp -o inject.exe \
    -lpsapi -static-libgcc -static-libstdc++
```
This injector:
1. Locates the target process (malware.exe)
2. Allocates memory in the target process
3. Injects defense.dll
4. Activates hooks inside the target process

