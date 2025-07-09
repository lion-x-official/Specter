#pragma once

#include "core/memory.hpp"

#include <cstdint>
#include <Windows.h>

// game offsets
namespace Offsets {
    extern DWORD64 client_dll;

    bool UpdateOffsets();
}