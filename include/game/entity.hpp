#pragma once

#include <Windows.h>
#include <vector>
#include "core/ProcessHelper.hpp"  // Directly include ProcessHelper
#include "valve/offsets.hpp"

// Maximum number of players in CS2
constexpr uint8_t MAX_PLAYERS = 64;

namespace EntityVars {
    struct EntityData {
        DWORD64 pawnAddress{ 0 };        // Address of the pawn entity
        DWORD64 controllerAddress{ 0 };  // Address of the controller entity
        int32_t health{ 0 };             // Current health of the entity
        int32_t maxHealth{ 0 };          // Maximum health of the entity
        uint8_t teamNum{ 0 };            // Team number of the entity
        uint32_t flags{ 0 };             // Entity flags (e.g., on ground, crouched)
        float position[3]{ 0.0f, 0.0f, 0.0f }; // Entity position (m_vOldOrigin)
    };

    // Global entity list
    inline std::vector<EntityData> entityList;
}

class Entity {
public:
    // Initialize entity system
    static bool Initialize(ProcessHelper& memory);

    // Update the entity list by reading from game memory
    static bool UpdateEntityList(ProcessHelper& memory);

    // Get the current entity list
    static const std::vector<EntityVars::EntityData>& GetEntityList();
};