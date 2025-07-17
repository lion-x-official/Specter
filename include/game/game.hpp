#pragma once

#include <Windows.h>

#include "core/ProcessHelper.hpp"
#include "utils/math.hpp"
#include "valve/offsets.hpp"
#include "utils/globals.hpp"

class Game{
public:
	static bool Initialize(ProcessHelper& memory);
	static bool Update(ProcessHelper& memory);
	static DWORD64 GetLocalPlayerPawn(ProcessHelper& memory);
};

namespace GameVars {
    namespace LocalPlayerPawn {
        inline uint64_t address{ 0 };
        inline int32_t health{ 0 };
        inline int32_t maxHealth{ 0 };
        inline uint8_t teamNum{ 0 };
        inline uint32_t flags{ 0 };
        inline Math::Vector3 position; // Changed to Vector3
    }
}