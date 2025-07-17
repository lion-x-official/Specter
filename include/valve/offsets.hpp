#pragma once

#include "core/ProcessHelper.hpp"

#include <cstdint>
#include <Windows.h>

// game offsets
namespace Offsets {
    namespace client_dll {
        namespace MainOffsets {
            constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x18590D0;
            constexpr std::ptrdiff_t dwLocalPlayerController = 0x1A53C38;
            constexpr std::ptrdiff_t dwEntityList = 0x1A05670;
            constexpr std::ptrdiff_t dwViewMatrix = 0x1A6E3F0;
        }

        namespace C_BaseEntity {
            constexpr std::ptrdiff_t m_iHealth = 0x344; // int32
            constexpr std::ptrdiff_t m_iMaxHealth = 0x340; // int32
            constexpr std::ptrdiff_t m_iTeamNum = 0x3E3; // uint8
            constexpr std::ptrdiff_t m_fFlags = 0x3EC; // uint32

            namespace C_BasePlayerPawn {
                constexpr std::ptrdiff_t m_iIDEntIndex = 0x1458; // CEntityIndex in entitylist
                constexpr std::ptrdiff_t m_vOldOrigin = 0x1324; // Vector
                constexpr std::ptrdiff_t m_hController = 0x133C; // CHandle<CBasePlayerController>
            }

            namespace CBasePlayerController {
                constexpr std::ptrdiff_t m_hPawn = 0x62C; // CHandle<C_BasePlayerPawn>
            }
        }
    }

    namespace engine2_dll {
        namespace MainOffsets {
            constexpr std::ptrdiff_t dwWindowHeight = 0x62359C;
            constexpr std::ptrdiff_t dwWindowWidth = 0x623598;
        }
    }
}