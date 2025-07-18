#pragma once

#include <Windows.h>
#include <vector>
#include <cstdint>
#include "utils/math.hpp"
#include "utils/globals.hpp"
#include "valve/offsets.hpp"

// Maximum number of players in CS2
constexpr uint8_t MAX_PLAYERS = 32;

// Structure to hold all entity data
struct EntityData {
    uint64_t controllerAddress{ 0 };
    uint64_t pawnAddress{ 0 };
    int32_t health{ 0 };
    int32_t maxHealth{ 0 };
    uint8_t teamNum{ 0 };
    uint32_t flags{ 0 };
    Math::Vector3 position;
    uint32_t pawnHandle{ 0 };
    // Add new fields here as needed
};

// Entity class to encapsulate entity data
class Entity {
public:
    // Constructor to initialize entity with data
    Entity(const EntityData& data) : data_(data) {}

    // Get entity data
    const EntityData& GetData() const { return data_; }

    // Check if the player is alive
    bool IsAlive() const { return data_.health > 0; }

    // Get entity ID index
    uint8_t GetEntIDIndex() const { return static_cast<uint8_t>(data_.pawnAddress & 0x7FFF); }

private:
    EntityData data_;
};

// Class to manage the list of entities
class EntityManager {
public:
    // Initialize entity system
    static bool Initialize();

    // Update the entity list by reading from game memory
    static bool UpdateEntityList();

    // Get the list of entities
    static const std::vector<Entity>& GetEntities();

private:
    // Global entity list
    inline static std::vector<Entity> entities_;
};