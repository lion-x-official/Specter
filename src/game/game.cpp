#include "game/game.hpp"
#include <iostream>

// Initialize game with process helper
bool Game::Initialize() {
    if (!Globals::processHelper->Attach()) {
        std::cerr << "Failed to attach to CS2 process" << std::endl;
        return false;
    }

    Globals::client_dll = Globals::processHelper->GetModuleBase(Globals::processHelper->GetProcessId(), L"client.dll");
    if (Globals::client_dll == 0) {
        std::cerr << "Failed to find client.dll" << std::endl;
        return false;
    }

    Globals::engine2_dll = Globals::processHelper->GetModuleBase(Globals::processHelper->GetProcessId(), L"engine2.dll");
    if (Globals::engine2_dll == 0) {
        std::cerr << "Failed to find engine2.dll" << std::endl;
        return false;
    }

    if (!Globals::processHelper->ReadMemory(Globals::engine2_dll + Offsets::engine2_dll::MainOffsets::dwWindowHeight, &Globals::screenHeight, sizeof(Globals::screenHeight)) ||
        !Globals::processHelper->ReadMemory(Globals::engine2_dll + Offsets::engine2_dll::MainOffsets::dwWindowWidth, &Globals::screenWidth, sizeof(Globals::screenWidth))) {
        std::cerr << "Failed to read screen dimensions" << std::endl;
        return false;
    }

    if (Globals::screenHeight == 0 || Globals::screenWidth == 0) {
        std::cerr << "Invalid screen dimensions" << std::endl;
        return false;
    }

    return true;
}

// Get local player pawn address
uint64_t Game::GetLocalPlayerPawn() {
    uint64_t localPlayerPawn = 0;
    if (!Globals::processHelper->ReadMemory(Globals::client_dll + Offsets::client_dll::MainOffsets::dwLocalPlayerPawn, &localPlayerPawn, sizeof(localPlayerPawn))) {
        std::cerr << "Failed to read local player pawn address" << std::endl;
        return 0;
    }
    return localPlayerPawn;
}

// Update game state
bool Game::Update() {
    uint64_t localPlayerPawn = 0;
    int32_t health = 0;
    int32_t maxHealth = 0;
    uint8_t teamNum = 0;
    uint32_t flags = 0;
    Math::Vector3 position;

    if (!Globals::processHelper->ReadMemory(Globals::client_dll + Offsets::client_dll::MainOffsets::dwLocalPlayerPawn, &localPlayerPawn, sizeof(localPlayerPawn)) || localPlayerPawn == 0) {
        std::cerr << "Failed to read local player pawn" << std::endl;
        LocalPlayerPawn::address = 0;
        LocalPlayerPawn::health = 0;
        LocalPlayerPawn::maxHealth = 0;
        LocalPlayerPawn::teamNum = 0;
        LocalPlayerPawn::flags = 0;
        LocalPlayerPawn::position = Math::Vector3();
        return false;
    }

    if (!Globals::processHelper->ReadMemory(localPlayerPawn + Offsets::client_dll::C_BaseEntity::m_iHealth, &health, sizeof(health)) ||
        !Globals::processHelper->ReadMemory(localPlayerPawn + Offsets::client_dll::C_BaseEntity::m_iMaxHealth, &maxHealth, sizeof(maxHealth)) ||
        !Globals::processHelper->ReadMemory(localPlayerPawn + Offsets::client_dll::C_BaseEntity::m_iTeamNum, &teamNum, sizeof(teamNum)) ||
        !Globals::processHelper->ReadMemory(localPlayerPawn + Offsets::client_dll::C_BaseEntity::m_fFlags, &flags, sizeof(flags)) ||
        !Globals::processHelper->ReadMemory(localPlayerPawn + Offsets::client_dll::C_BaseEntity::C_BasePlayerPawn::m_vOldOrigin, &position, sizeof(position))) {
        std::cerr << "Failed to read local player data" << std::endl;
        LocalPlayerPawn::address = 0;
        LocalPlayerPawn::health = 0;
        LocalPlayerPawn::maxHealth = 0;
        LocalPlayerPawn::teamNum = 0;
        LocalPlayerPawn::flags = 0;
        LocalPlayerPawn::position = Math::Vector3();
        return false;
    }

    LocalPlayerPawn::address = localPlayerPawn;
    LocalPlayerPawn::health = health;
    LocalPlayerPawn::maxHealth = maxHealth;
    LocalPlayerPawn::teamNum = teamNum;
    LocalPlayerPawn::flags = flags;
    LocalPlayerPawn::position = position;

    return true;
}