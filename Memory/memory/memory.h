#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <cstdint>
#include <string>

class Memory
{
private:
	std::uintptr_t procID = 0;
	HANDLE procHandle = nullptr;

public:
	Memory(std::wstring procName);
	~Memory();

	std::uintptr_t getModuleBase(const std::wstring moduleName) const;

	template <typename T>
	T Read(const std::uintptr_t& address) const
	{
		T value{ };
		ReadProcessMemory(procHandle, reinterpret_cast<const void*>(address), &value, sizeof(T), NULL);
		return value;
	}

	template <typename T>
	void Write(const std::uintptr_t& address, const T& value) const
	{
		WriteProcessMemory(procHandle, reinterpret_cast<void*>(address), &value, sizeof(T), NULL);
	}
};