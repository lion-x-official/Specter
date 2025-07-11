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
	return localPlayerPawn; // Return the local player pawn address
}

bool Game::Update(ProcessHelper& memory) {
    DWORD64 localPlayerPawn = NULL;
	int32_t health = NULL;
    int32_t maxHealth = NULL;
    uint8_t teamNum = NULL;
    uint32_t flags = NULL;

    memory.ReadMemory(Offsets::client_dll + Offsets::dwLocalPlayerPawn, &localPlayerPawn, sizeof(localPlayerPawn));
    if (localPlayerPawn > 0) {
        memory.ReadMemory(localPlayerPawn + Offsets::m_iHealth, &health, sizeof(health));
        memory.ReadMemory(localPlayerPawn + Offsets::m_iMaxHealth, &maxHealth, sizeof(maxHealth));
        memory.ReadMemory(localPlayerPawn + Offsets::m_iTeamNum, &teamNum, sizeof(teamNum));
        memory.ReadMemory(localPlayerPawn + Offsets::m_fFlags, &flags, sizeof(flags));
        
        GameVars::LocalPlayerPawn::health = health;
        GameVars::LocalPlayerPawn::maxHealth = maxHealth;
        GameVars::LocalPlayerPawn::teamNum = teamNum;
        GameVars::LocalPlayerPawn::flags = flags;
    }
    else {
        GameVars::LocalPlayerPawn::health = 0;
        GameVars::LocalPlayerPawn::maxHealth = 0;
        GameVars::LocalPlayerPawn::teamNum = 0;
        GameVars::LocalPlayerPawn::flags = 0;
    }
    return localPlayerPawn != 0; // Return true if local player pawn is valid
}