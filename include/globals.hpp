#pragma once
#include <Windows.h>
#include <memory.hpp>

template <typename T>
inline bool GetAddressWithOffset(const DWORD64& address, DWORD offset, T& value) {
	if (address == 0) return false;

	if (Memory::ReadMemory(address, &value, sizeof(T)) == false) {
		return false; // Failed to read memory
	}
	return true;
}