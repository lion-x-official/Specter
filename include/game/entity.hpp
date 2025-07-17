#pragma once

#include <Windows.h>
#include <vector>
#include <cstdint>
#include "core/ProcessHelper.hpp"  // Directly include ProcessHelper
#include "utils/math.hpp"          // For Vector3
#include "utils/globals.hpp"       // For global variables
#include "valve/offsets.hpp"

// Maximum number of players in CS2
constexpr uint8_t MAX_PLAYERS = 64;

namespace EntityVars {
    // Structure to hold raw entity data
    struct EntityData {
        uint64_t pawnAddress{ 0 };       // Address of the pawn entity
        uint64_t controllerAddress{ 0 }; // Address of the controller entity
        int32_t health{ 0 };             // Current health of the entity
        int32_t maxHealth{ 0 };          // Maximum health of the entity
        uint8_t teamNum{ 0 };            // Team number of the entity
        uint32_t flags{ 0 };             // Entity flags (e.g., on ground, crouched)
        Math::Vector3 position;          // Entity position (m_vOldOrigin)
    };
}

// Class representing a single entity
class Entity {
public:
    Entity(const EntityVars::EntityData& data) : data_(data) {}

    // Get health of the entity
    int32_t GetHealth() const { return data_.health; }

    // Get maximum health of the entity
    int32_t GetMaxHealth() const { return data_.maxHealth; }

    // Get team number of the entity
    uint8_t GetTeamNum() const { return data_.teamNum; }

    // Get entity flags
    uint32_t GetFlags() const { return data_.flags; }

    // Get position of the entity
    Math::Vector3 GetPosition() const { return data_.position; }

    // Get pawn address
    uint64_t GetPawnAddress() const { return data_.pawnAddress; }

    // Get controller address
    uint64_t GetControllerAddress() const { return data_.controllerAddress; }

    // Check if entity is valid
    bool IsValid() const { return data_.pawnAddress != 0 && data_.health > 0; }

private:
    EntityVars::EntityData data_; // Raw entity data
};

// Class to manage the list of entities
class EntityManager {
public:
    // Initialize entity system
    static bool Initialize(ProcessHelper& memory);

    // Update the entity list by reading from game memory
    static bool UpdateEntityList(ProcessHelper& memory);

    // Get the list of entities
    static const std::vector<Entity>& GetEntities();

    // Get pawn address from entity index
    static uint64_t GetEntityPawnFromIndex(ProcessHelper& memory, int index);

    // Get controller address from entity index
    static uint64_t GetEntityControllerFromIndex(ProcessHelper& memory, int index);

private:
    // Global entity list
    inline static std::vector<Entity> entities_;
};