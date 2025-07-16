#pragma once

#include "core/ProcessHelper.hpp"

#include <cstdint>
#include <Windows.h>

// game offsets
namespace Offsets {
    inline uintptr_t client_dll = 0;

    namespace MainOffsets {
        constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x18580D0;
        constexpr std::ptrdiff_t dwLocalPlayerController = 0x1A52D20;
        constexpr std::ptrdiff_t dwEntityList = 0x1A044E0;
    }

    namespace Pawn {
        constexpr std::ptrdiff_t m_iHealth = 0x344; // int32
        constexpr std::ptrdiff_t m_iMaxHealth = 0x340; // int32
        constexpr std::ptrdiff_t m_iTeamNum = 0x3E3; // uint8
        constexpr std::ptrdiff_t m_fFlags = 0x3EC; // uint32
        constexpr std::ptrdiff_t m_iIDEntIndex = 0x1458; // CEntityIndex in entitylist
        constexpr std::ptrdiff_t m_vOldOrigin = 0x1324; // Vector
    }
    
    namespace Controller{
        constexpr std::ptrdiff_t m_hPawn = 0x62C; // CHandle<C_BasePlayerPawn>
    }
}