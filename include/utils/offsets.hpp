#pragma once

#include "core/ProcessHelper.hpp"

#include <cstdint>
#include <Windows.h>

// game offsets
namespace Offsets {
    inline uintptr_t client_dll = 0;

    constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x18580D0;
    constexpr std::ptrdiff_t dwLocalPlayerController = 0x1A52D20;
    constexpr std::ptrdiff_t dwEntityList = 0x1A044E0;

    bool UpdateOffsets(ProcessHelper& memory);
}