#pragma once

#include "core/ProcessHelper.hpp"

#include <cstdint>
#include <Windows.h>

// game offsets
namespace Offsets {
    inline uintptr_t client_dll = 0;

    bool UpdateOffsets(ProcessHelper& memory);
}