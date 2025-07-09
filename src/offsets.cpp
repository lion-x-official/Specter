#include <offsets.hpp>

bool Offsets::UpdateOffsets()
{
	Offsets::client_dll = Memory(L"cs2.exe").GetModuleBase(Memory(L"cs2.exe").GetProcessId(), L"client.dll");
	if (Offsets::client_dll == 0) return false;

    return true;
}