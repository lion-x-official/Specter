#include "game/entity.hpp"
#include <iostream>
#include <sstream>

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

// Structure to hold entity properties for bulk reading
struct EntityProperties {
    int32_t health;
    int32_t maxHealth;
    uint8_t teamNum;
    uint32_t flags;
    Math::Vector3 position;
    // Add new fields here as needed
};

// Initialize entity system
bool EntityManager::Initialize() {
    // Check if process is running and attach to it
    ProcessHelper& memory = *Globals::processHelper;

    if (!memory.IsProcessRunning() || !memory.Attach()) {
        std::cerr << "Failed to attach to CS2 process (PID: " << memory.GetProcessId() << ")" << std::endl;
        return false;
    }

    // Initialize global process helper and entity list
    {
        std::lock_guard<std::mutex> lock(Globals::globalMutex);
        if (!Globals::processHelper) {
            Globals::processHelper = &memory;
        }
        Globals::entityList = &entities_;
        Globals::cs2Running = true;
    }
    return true;
}

// Update the entity list by reading from game memory
bool EntityManager::UpdateEntityList() {
    ProcessHelper& memory = *Globals::processHelper;
    std::vector<Entity> tempEntities;
    tempEntities.reserve(MAX_PLAYERS);

    // Read entity list base address
    uint64_t entityList{ 0 };
    if (!memory.ReadMemory(Globals::client_dll + Offsets::client_dll::MainOffsets::dwEntityList, &entityList, sizeof(entityList)) || entityList == 0) {
        std::cerr << "Failed to read entity list base address at 0x" << std::hex << (Globals::client_dll + Offsets::client_dll::MainOffsets::dwEntityList) << std::dec << std::endl;
        std::lock_guard<std::mutex> lock(Globals::globalMutex);
        entities_.clear();
        Globals::entityList = &entities_;
        Globals::cs2Running = false;
        return false;
    }

    // Iterate through players
    for (int i = 0; i < MAX_PLAYERS; ++i) {
        uint64_t listEntry{ 0 };
        const uintptr_t listEntryAddress = entityList + (Offsets::EntityList::ENTRY_OFFSET * (i & Offsets::EntityList::INDEX_MASK) >> 9) + Offsets::EntityList::LIST_OFFSET;
        if (!memory.ReadMemory(listEntryAddress, &listEntry, sizeof(listEntry)) || listEntry == 0) {
            continue;
        }

        uint64_t entityController{ 0 };
        if (!memory.ReadMemory(listEntry + Offsets::EntityList::ENTRY_INDEX * (i & Offsets::EntityList::CONTROLLER_MASK), &entityController, sizeof(entityController)) || entityController == 0) {
            continue;
        }

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

        // Read entity properties in one go
        EntityProperties props;
        if (!memory.ReadMemory(entityPawn + Offsets::client_dll::C_BaseEntity::m_iHealth, &props, sizeof(EntityProperties))) {
            std::cerr << "Failed to read entity properties at 0x" << std::hex << entityPawn << std::dec << " for index " << i << std::endl;
            continue;
        }

        // Create entity data
        EntityData data;
        data.controllerAddress = entityController;
        data.pawnAddress = entityPawn;
        data.health = props.health;
        data.maxHealth = props.maxHealth;
        data.teamNum = props.teamNum;
        data.flags = props.flags;
        data.position = props.position;
        data.pawnHandle = static_cast<uint32_t>(entityControllerPawn & 0xFFFFFFFF);
        // Add new fields here as needed

        // Add entity to temporary list
        tempEntities.emplace_back(data);
    }

    // Update global entity list
    {
        std::lock_guard<std::mutex> lock(Globals::globalMutex);
        entities_ = std::move(tempEntities);
        Globals::entityList = &entities_;
        Globals::cs2Running = !entities_.empty();
    }

    return !entities_.empty();
}

// Get the list of entities
const std::vector<Entity>& EntityManager::GetEntities() {
    return entities_;
}