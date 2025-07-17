#include "game/game.hpp"

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
    memory.ReadMemory(Offsets::client_dll + Offsets::MainOffsets::dwLocalPlayerPawn, &localPlayerPawn, sizeof(localPlayerPawn));
	return localPlayerPawn; // Return the local player pawn address
}

bool Game::Update(ProcessHelper& memory) {
    uint64_t localPlayerPawn = NULL;
    int32_t health = NULL;
    int32_t maxHealth = NULL;
    uint8_t teamNum = NULL;
    uint32_t flags = NULL;
    Math::Vector3 position; // Changed to Vector3

    memory.ReadMemory(Offsets::client_dll + Offsets::MainOffsets::dwLocalPlayerPawn, &localPlayerPawn, sizeof(localPlayerPawn));
    if (localPlayerPawn > 0) {
        memory.ReadMemory(localPlayerPawn + Offsets::Pawn::m_iHealth, &health, sizeof(health));
        memory.ReadMemory(localPlayerPawn + Offsets::Pawn::m_iMaxHealth, &maxHealth, sizeof(maxHealth));
        memory.ReadMemory(localPlayerPawn + Offsets::Pawn::m_iTeamNum, &teamNum, sizeof(teamNum));
        memory.ReadMemory(localPlayerPawn + Offsets::Pawn::m_fFlags, &flags, sizeof(flags));
        memory.ReadMemory(localPlayerPawn + Offsets::Pawn::m_vOldOrigin, &position, sizeof(position));

        GameVars::LocalPlayerPawn::address = localPlayerPawn;
        GameVars::LocalPlayerPawn::health = health;
        GameVars::LocalPlayerPawn::maxHealth = maxHealth;
        GameVars::LocalPlayerPawn::teamNum = teamNum;
        GameVars::LocalPlayerPawn::flags = flags;
        GameVars::LocalPlayerPawn::position = position;
    }
    // ... rest of the code
    else {
        GameVars::LocalPlayerPawn::address = NULL;
        GameVars::LocalPlayerPawn::health = NULL;
        GameVars::LocalPlayerPawn::maxHealth = NULL;
        GameVars::LocalPlayerPawn::teamNum = NULL;
        GameVars::LocalPlayerPawn::flags = NULL;
        GameVars::LocalPlayerPawn::position = Math::Vector3();
    }
    return localPlayerPawn != 0; // Return true if local player pawn is valid
}