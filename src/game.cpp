#include "game.hpp"

bool Game::Initialize(ProcessHelper& memory)
{
	memory.Attach();
	Offsets::client_dll = memory.GetModuleBase(memory.GetProcessId(), L"client.dll");
	if (Offsets::client_dll == 0) return false;

	return true;
}

DWORD64 Game::GetLocalPlayerPawn(ProcessHelper& memory)
{
	DWORD64 localPlayerPawn = NULL;
	memory.ReadMemory(Offsets::client_dll + Offsets::dwLocalPlayerPawn, &localPlayerPawn, sizeof(localPlayerPawn));
	if (localPlayerPawn <= 0) return 0; // Ensure the local player pawn is valid
	return localPlayerPawn;  // Return localplayerpawn address
}
