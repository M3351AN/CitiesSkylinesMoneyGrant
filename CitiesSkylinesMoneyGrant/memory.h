#pragma once
#include <windows.h>
#include <TlHelp32.h>
#define WIN32_LEAN_AND_MEAN


namespace MEM {
	DWORD GetProcess(const wchar_t* Target) {
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

	uintptr_t GetModuleBaseAddress(DWORD processId, const wchar_t* ModuleTarget) {

		HANDLE snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);

		if (snapshotHandle == INVALID_HANDLE_VALUE) {
			return NULL;
		}

		MODULEENTRY32W moduleEntry = { };
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

	DWORD processId = GetProcess(L"Cities.exe");

	uintptr_t baseAddress = GetModuleBaseAddress(processId, L"mono.dll");

	HANDLE processHandle = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, processId);

	template <typename T>
	T ReadAddress(uintptr_t ptrAddress) {
		T value = { };
		ReadProcessMemory(MEM::processHandle, (LPVOID)ptrAddress, &value, sizeof(T), NULL);
		return value;
	}

	template <typename T>
	bool WriteAddress(uintptr_t ptrAddress, T value) {

		return WriteProcessMemory(MEM::processHandle, (LPVOID)ptrAddress, &value, sizeof(T), NULL);

	}

}