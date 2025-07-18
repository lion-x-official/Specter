#pragma once

#include <Windows.h>

#include "core/ProcessHelper.hpp"
#include "utils/math.hpp"
#include "valve/offsets.hpp"
#include "utils/globals.hpp"

static class Game{
public:
	static bool Initialize(ProcessHelper& memory);
	static bool Update(ProcessHelper& memory);
	static DWORD64 GetLocalPlayerPawn(ProcessHelper& memory);

    struct LocalPlayerPawn {
        static inline uint64_t address{ 0 };
        static inline int32_t health{ 0 };
        static inline int32_t maxHealth{ 0 };
        static inline uint8_t teamNum{ 0 };
        static inline uint32_t flags{ 0 };
        static inline Math::Vector3 position;
    };
};