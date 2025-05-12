#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <sstream>
#include <TlHelp32.h>
#include <psapi.h>
#define WIN32_LEAN_AND_MEAN

namespace Ukia {

    DWORD GetProc(const wchar_t* Target) {
        HANDLE snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

        if (snapshotHandle == INVALID_HANDLE_VALUE) {
            return NULL;
        }

        PROCESSENTRY32W processEntry = { };
        processEntry.dwSize = sizeof(PROCESSENTRY32W);

        if (Process32FirstW(snapshotHandle, &processEntry)) {

            do {

                if (_wcsicmp(processEntry.szExeFile, Target) == 0) {
                    CloseHandle(snapshotHandle);
                    return processEntry.th32ProcessID;
                }

            } while (Process32NextW(snapshotHandle, &processEntry));

        }

        CloseHandle(snapshotHandle);
        return NULL;

    }

    MODULEENTRY32W GetModuleBase(DWORD processId, const wchar_t* ModuleTarget) {

        HANDLE snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);

        MODULEENTRY32W moduleEntry = { };

        if (snapshotHandle == INVALID_HANDLE_VALUE) {
            MODULEENTRY32W module = { 0UL };
            return module;
        }


        moduleEntry.dwSize = sizeof(MODULEENTRY32W);

        if (Module32FirstW(snapshotHandle, &moduleEntry)) {

            do {

                if (_wcsicmp(moduleEntry.szModule, ModuleTarget) == 0) {
                    CloseHandle(snapshotHandle);
                    return moduleEntry;
                }

            } while (Module32NextW(snapshotHandle, &moduleEntry));

        }

        CloseHandle(snapshotHandle);
        MODULEENTRY32W module = { 0UL };
        return module;

    }

    uintptr_t GetModuleBaseAddr(DWORD processId, const wchar_t* ModuleTarget) {

        HANDLE snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);

        MODULEENTRY32W moduleEntry = { };

        if (snapshotHandle == INVALID_HANDLE_VALUE) {
            return NULL;
        }

        moduleEntry.dwSize = sizeof(MODULEENTRY32W);

        if (Module32FirstW(snapshotHandle, &moduleEntry)) {

            do {

                if (_wcsicmp(moduleEntry.szModule, ModuleTarget) == 0) {
                    CloseHandle(snapshotHandle);
                    return reinterpret_cast<uintptr_t>(moduleEntry.modBaseAddr);
                }

            } while (Module32NextW(snapshotHandle, &moduleEntry));

        }

        CloseHandle(snapshotHandle);
        return NULL;

    }

    template <typename T>
    T ReadAddr(HANDLE processHandle, uintptr_t ptrAddress) {
        T value = { };
        ReadProcessMemory(processHandle, (LPVOID)ptrAddress, &value, sizeof(T), NULL);
        return value;
    }

    template <typename T>
    bool WriteAddr(HANDLE processHandle, uintptr_t ptrAddress, T value) {

        return WriteProcessMemory(processHandle, (LPVOID)ptrAddress, &value, sizeof(T), NULL);

    }

    template<typename T>
    T DeRefPtr(HANDLE processHandle, uintptr_t baseAddr, const std::vector<uintptr_t>& offset) {
        uintptr_t addr = baseAddr;
        for (size_t i = 0; i < offset.size(); i++) {

            addr = ReadAddr<uintptr_t>(processHandle, addr);
            addr += offset[offset.size() - i - 1];//The ptr offsets in CT tablez are in reverse order.
        }

        return addr;
    }

}