#include "game/entity.hpp"
#include <iostream> // For logging

// Constants for entity list offsets
namespace Offsets {
    namespace EntityList {
        constexpr size_t ENTRY_OFFSET = 8;
        constexpr size_t LIST_OFFSET = 16;
        constexpr size_t INDEX_MASK = 0x7FFF;
        constexpr size_t ENTRY_INDEX = 120;
        constexpr size_t CONTROLLER_MASK = 0x1FF;
    }
}

bool EntityManager::Initialize(ProcessHelper& memory) {
    // Check if process is running and attach to it
    if (!memory.IsProcessRunning() || !memory.Attach()) {
        std::cerr << "Failed to attach to CS2 process" << std::endl;
        return false;
    }

    // Initialize global process helper and entity list
    {
        std::lock_guard<std::mutex> lock(Globals::globalMutex);
        if (!Globals::processHelper) {
            Globals::processHelper = &memory;
        }
        // Initialize global entity list pointer to point to EntityManager::entities_
        Globals::entityList = &entities_;
        Globals::cs2Running = true;
    }
    return true;
}

bool EntityManager::UpdateEntityList(ProcessHelper& memory) {
    // Reserve memory for entity list to avoid reallocations
    {
        std::lock_guard<std::mutex> lock(Globals::globalMutex);
        entities_.clear();
        entities_.reserve(MAX_PLAYERS);
        // Ensure global entity list points to the updated list
        Globals::entityList = &entities_;
    }

    // Read entity list base address
    uint64_t entityList{ 0 };
    if (!memory.ReadMemory(Globals::client_dll + Offsets::client_dll::MainOffsets::dwEntityList, &entityList, sizeof(entityList)) || entityList == 0) {
        std::cerr << "Failed to read entity list base address" << std::endl;
        std::lock_guard<std::mutex> lock(Globals::globalMutex);
        Globals::cs2Running = false;
        return false;
    }

    // Iterate through players (max 64 players, standard for CS2)
    for (int i = 0; i < MAX_PLAYERS; ++i) {
        uint64_t listEntry{ 0 };
        if (!memory.ReadMemory(entityList + (Offsets::EntityList::ENTRY_OFFSET * (i & Offsets::EntityList::INDEX_MASK) >> 9) + Offsets::EntityList::LIST_OFFSET, &listEntry, sizeof(listEntry)) || listEntry == 0) {
            continue;
        }

        EntityVars::EntityData entityData;
        uint64_t entityController{ 0 };
        if (!memory.ReadMemory(listEntry + Offsets::EntityList::ENTRY_INDEX * (i & Offsets::EntityList::CONTROLLER_MASK), &entityController, sizeof(entityController)) || entityController == 0) {
            continue;
        }

        entityData.controllerAddress = entityController;

        uint64_t entityControllerPawn{ 0 };
        if (!memory.ReadMemory(entityController + Offsets::client_dll::C_BaseEntity::CBasePlayerController::m_hPawn, &entityControllerPawn, sizeof(entityControllerPawn)) || entityControllerPawn == 0) {
            continue;
        }

        if (!memory.ReadMemory(entityList + (Offsets::EntityList::ENTRY_OFFSET * ((entityControllerPawn & Offsets::EntityList::INDEX_MASK) >> 9) + Offsets::EntityList::LIST_OFFSET), &listEntry, sizeof(listEntry)) || listEntry == 0) {
            continue;
        }

        uint64_t entityPawn{ 0 };
        if (!memory.ReadMemory(listEntry + Offsets::EntityList::ENTRY_INDEX * (entityControllerPawn & Offsets::EntityList::CONTROLLER_MASK), &entityPawn, sizeof(entityPawn)) || entityPawn == 0) {
            continue;
        }

        entityData.pawnAddress = entityPawn;

        // Read entity properties
        if (!memory.ReadMemory(entityPawn + Offsets::client_dll::C_BaseEntity::m_iHealth, &entityData.health, sizeof(entityData.health)) ||
            !memory.ReadMemory(entityPawn + Offsets::client_dll::C_BaseEntity::m_iMaxHealth, &entityData.maxHealth, sizeof(entityData.maxHealth)) ||
            !memory.ReadMemory(entityPawn + Offsets::client_dll::C_BaseEntity::m_iTeamNum, &entityData.teamNum, sizeof(entityData.teamNum)) ||
            !memory.ReadMemory(entityPawn + Offsets::client_dll::C_BaseEntity::m_fFlags, &entityData.flags, sizeof(entityData.flags)) ||
            !memory.ReadMemory(entityPawn + Offsets::client_dll::C_BaseEntity::C_BasePlayerPawn::m_vOldOrigin, &entityData.position, sizeof(Math::Vector3))) {
            continue;
        }

        // Add entity to the list
        {
            std::lock_guard<std::mutex> lock(Globals::globalMutex);
            entities_.emplace_back(entityData);
        }
    }

    // Update global CS2 process status
    {
        std::lock_guard<std::mutex> lock(Globals::globalMutex);
        Globals::cs2Running = !entities_.empty();
    }

    return !entities_.empty();
}

const std::vector<Entity>& EntityManager::GetEntities() {
    return entities_;
}