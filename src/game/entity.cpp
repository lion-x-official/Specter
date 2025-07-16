#include "game/entity.hpp"
#include "utils/globals.hpp"  // Include globals.hpp here to access Globals::entityList

bool Entity::Initialize(ProcessHelper& memory) {
    // Ensure the process is attached and client.dll is found
    if (!memory.IsProcessRunning() || !memory.Attach()) {
        return false;
    }

    // Initialize global process helper and entity list
    {
        std::lock_guard<std::mutex> lock(Globals::globalMutex);
        Globals::processHelper = &memory;
        Globals::entityList = &EntityVars::entityList; // Initialize the pointer
        Globals::cs2Running = true;
    }
    return true;
}

bool Entity::UpdateEntityList(ProcessHelper& memory) {
    // Clear the entity list
    {
        std::lock_guard<std::mutex> lock(Globals::globalMutex);
        EntityVars::entityList.clear();
    }

    // Read the entity list base address
    DWORD64 entityList{ 0 };
    if (!memory.ReadMemory(Offsets::client_dll + Offsets::MainOffsets::dwEntityList, &entityList, sizeof(entityList)) || entityList == 0) {
        return false;
    }

    // Iterate through players (max 64 players, standard for CS2)
    for (int i = 0; i < MAX_PLAYERS; ++i) {
        DWORD64 listEntry{ 0 };
        if (!memory.ReadMemory(entityList + ((8 * (i & 0x7FFF) >> 9) + 16), &listEntry, sizeof(listEntry)) || listEntry == 0) {
            continue;
        }

        EntityVars::EntityData entityData;
        DWORD64 entityController{ 0 };
        if (!memory.ReadMemory(listEntry + (120) * (i & 0x1FF), &entityController, sizeof(entityController)) || entityController == 0) {
            continue;
        }

        entityData.controllerAddress = entityController;

        DWORD64 entityControllerPawn{ 0 };
        if (!memory.ReadMemory(entityController + Offsets::Controller::m_hPawn, &entityControllerPawn, sizeof(entityControllerPawn)) || entityControllerPawn == 0) {
            continue;
        }

        if (!memory.ReadMemory(entityList + (0x8 * ((entityControllerPawn & 0x7FFF) >> 9) + 16), &listEntry, sizeof(listEntry)) || listEntry == 0) {
            continue;
        }

        DWORD64 entityPawn{ 0 };
        if (!memory.ReadMemory(listEntry + (120) * (entityControllerPawn & 0x1FF), &entityPawn, sizeof(entityPawn)) || entityPawn == 0) {
            continue;
        }

        entityData.pawnAddress = entityPawn;

        // Read entity properties
        if (!memory.ReadMemory(entityPawn + Offsets::Pawn::m_iHealth, &entityData.health, sizeof(entityData.health)) ||
            !memory.ReadMemory(entityPawn + Offsets::Pawn::m_iMaxHealth, &entityData.maxHealth, sizeof(entityData.maxHealth)) ||
            !memory.ReadMemory(entityPawn + Offsets::Pawn::m_iTeamNum, &entityData.teamNum, sizeof(entityData.teamNum)) ||
            !memory.ReadMemory(entityPawn + Offsets::Pawn::m_fFlags, &entityData.flags, sizeof(entityData.flags)) ||
            !memory.ReadMemory(entityPawn + Offsets::Pawn::m_vOldOrigin, entityData.position, sizeof(entityData.position))) {
            continue;
        }

        // Add entity to the list
        {
            std::lock_guard<std::mutex> lock(Globals::globalMutex);
            EntityVars::entityList.push_back(entityData);
        }
    }

    // Update global CS2 process status
    {
        std::lock_guard<std::mutex> lock(Globals::globalMutex);
        Globals::cs2Running = !EntityVars::entityList.empty();
    }

    return !EntityVars::entityList.empty();
}

const std::vector<EntityVars::EntityData>& Entity::GetEntityList() {
    return EntityVars::entityList;
}