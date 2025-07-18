#pragma once

#include <Windows.h>
#include "utils/math.hpp"
#include "valve/offsets.hpp"
#include "utils/globals.hpp"

// Game class to manage game-related operations
class Game {
public:
    // Initialize game with process helper
    static bool Initialize();

    // Update game state
    static bool Update();

    // Get local player pawn address
    static uint64_t GetLocalPlayerPawn();

    // Structure to hold local player pawn data
    struct LocalPlayerPawn {
        static inline uint64_t address{ 0 };
        static inline int32_t health{ 0 };
        static inline int32_t maxHealth{ 0 };
        static inline uint8_t teamNum{ 0 };
        static inline uint32_t flags{ 0 };
        static inline Math::Vector3 position;
    };
};