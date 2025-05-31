#include "memory.h"

#include <TlHelp32.h>
#include <iostream>

Memory::Memory(const std::wstring procName)
{
	PROCESSENTRY32 entry{ };
	entry.dwSize = sizeof(PROCESSENTRY32);

	const HANDLE snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (snapShot == INVALID_HANDLE_VALUE)
	{
		std::cout << "Failed to create process snapshot.";
		Sleep(2500);
		exit(1);
	}

	if (Process32First(snapShot, &entry))
	{
		do
		{
			if (!procName.compare(entry.szExeFile))
			{
				procID = entry.th32ProcessID;
				procHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);
				break;
			}
		}
		while (Process32Next(snapShot, &entry));
	}

	if (!procHandle)
	{
		std::cout << "Failed to open process.";
		Sleep(2500);
		if (snapShot != INVALID_HANDLE_VALUE)
			CloseHandle(snapShot);
		exit(1);
	}

	if (snapShot != INVALID_HANDLE_VALUE)
		CloseHandle(snapShot);
}

Memory::~Memory()
{
	if (procHandle)
		CloseHandle(procHandle);
}

std::uintptr_t Memory::getModuleBase(const std::wstring moduleName) const
{
	MODULEENTRY32 entry{ };
	entry.dwSize = sizeof(MODULEENTRY32);

	const HANDLE snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, procID);

	if (snapShot == INVALID_HANDLE_VALUE)
	{
		std::cout << "Failed to create module snapshot.";
		Sleep(2500);
		exit(1);
	}

	std::uintptr_t moduleBaseAddress = 0;

	if (Module32First(snapShot, &entry))
	{
		do
		{
			if (!moduleName.compare(entry.szModule))
			{
				moduleBaseAddress = reinterpret_cast<std::uintptr_t>(entry.modBaseAddr);
				break;
			}
		}
		while (Module32Next(snapShot, &entry));
	}

	if (snapShot != INVALID_HANDLE_VALUE)
		CloseHandle(snapShot);

	return moduleBaseAddress;
}