#include <offsets.hpp>

bool Offsets::UpdateOffsets(Memory &memory)
{
	uint32_t pId = memory.GetProcessId();
    if (pId == 0) return false;

    Offsets::client_dll = memory.GetModuleBase(pId, L"client.dll");
    if (Offsets::client_dll == 0) return false;

    return true;
}