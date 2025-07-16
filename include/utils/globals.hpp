#pragma once

#include <mutex>
#include <cstdint>
#include <vector>
#include "core/ProcessHelper.hpp"
#include "game/game.hpp"
#include "game/entity.hpp"

namespace Globals {
    // Mutex for thread-safe access to global data
    inline std::mutex globalMutex;

    // Pointer to ProcessHelper for CS2 process interaction
    inline ProcessHelper* processHelper = nullptr;

    // Pointer to entity list (updated by Entity::UpdateEntityList)
    inline std::vector<EntityVars::EntityData>* entityList = nullptr;

    // Screen resolution parameters (to be set from game data)
    inline uint32_t screenWidth = 0;
    inline uint32_t screenHeight = 0;

    // Mathematical constants
    inline constexpr float PI = 3.141592653589793f;
    inline constexpr float DEG_TO_RAD = PI / 180.0f;
    inline constexpr float RAD_TO_DEG = 180.0f / PI;

    // CS2 process information
    inline uint32_t cs2ProcessId = 0;
    inline bool cs2Running = false;

    // Cheat features
    inline bool triggerEnabled = false;

}