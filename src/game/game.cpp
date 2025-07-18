#include "game/game.hpp"


bool Game::Initialize(ProcessHelper& memory)
{
	memory.Attach();
	Globals::client_dll = memory.GetModuleBase(memory.GetProcessId(), L"client.dll");
	if (Globals::client_dll == 0) return false;

	Globals::engine2_dll = memory.GetModuleBase(memory.GetProcessId(), L"engine2.dll");
	if (Globals::engine2_dll == 0) return false;

    Globals::screenHeight = 0;
	Globals::screenWidth = 0;
    memory.ReadMemory(Globals::engine2_dll + Offsets::engine2_dll::MainOffsets::dwWindowHeight, &Globals::screenHeight, sizeof(Globals::screenHeight));
	memory.ReadMemory(Globals::engine2_dll + Offsets::engine2_dll::MainOffsets::dwWindowWidth, &Globals::screenWidth, sizeof(Globals::screenWidth));
    if (Globals::screenHeight == 0 || Globals::screenWidth == 0) {
        return false; // Failed to read screen dimensions
	}

	return true;
}

DWORD64 Game::GetLocalPlayerPawn(ProcessHelper& memory)
{
	DWORD64 localPlayerPawn = NULL;
    memory.ReadMemory(Globals::client_dll + Offsets::client_dll::MainOffsets::dwLocalPlayerPawn, &localPlayerPawn, sizeof(localPlayerPawn));
	return localPlayerPawn; // Return the local player pawn address
}

bool Game::Update(ProcessHelper& memory) {
    uint64_t localPlayerPawn = NULL;
    int32_t health = NULL;
    int32_t maxHealth = NULL;
    uint8_t teamNum = NULL;
    uint32_t flags = NULL;
    Math::Vector3 position; // Changed to Vector3

    memory.ReadMemory(Globals::client_dll + Offsets::client_dll::MainOffsets::dwLocalPlayerPawn, &localPlayerPawn, sizeof(localPlayerPawn));
    if (localPlayerPawn > 0) {
        memory.ReadMemory(localPlayerPawn + Offsets::client_dll::C_BaseEntity::m_iHealth, &health, sizeof(health));
        memory.ReadMemory(localPlayerPawn + Offsets::client_dll::C_BaseEntity::m_iMaxHealth, &maxHealth, sizeof(maxHealth));
        memory.ReadMemory(localPlayerPawn + Offsets::client_dll::C_BaseEntity::m_iTeamNum, &teamNum, sizeof(teamNum));
        memory.ReadMemory(localPlayerPawn + Offsets::client_dll::C_BaseEntity::m_fFlags, &flags, sizeof(flags));
		memory.ReadMemory(localPlayerPawn + Offsets::client_dll::C_BaseEntity::C_BasePlayerPawn::m_vOldOrigin, &position, sizeof(position));

        Game::LocalPlayerPawn::address = localPlayerPawn;
        Game::LocalPlayerPawn::health = health;
        Game::LocalPlayerPawn::maxHealth = maxHealth;
        Game::LocalPlayerPawn::teamNum = teamNum;
        Game::LocalPlayerPawn::flags = flags;
        Game::LocalPlayerPawn::position = position;
    }
    else {
        Game::LocalPlayerPawn::address = NULL;
        Game::LocalPlayerPawn::health = NULL;
        Game::LocalPlayerPawn::maxHealth = NULL;
        Game::LocalPlayerPawn::teamNum = NULL;
        Game::LocalPlayerPawn::flags = NULL;
        Game::LocalPlayerPawn::position = Math::Vector3();
    }
    return localPlayerPawn != 0; // Return true if local player pawn is valid
}