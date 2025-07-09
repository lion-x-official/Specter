#pragma once

#include "core/memory.hpp"

#include <cstdint>
#include <Windows.h>

// game offsets
namespace Offsets {
    inline uintptr_t client_dll = 0;

    bool UpdateOffsets(Memory &memory);
}