#pragma once

#include <Windows.h>

#include "core/ProcessHelper.hpp"
#include "valve/offsets.hpp"

class Game{
public:
	static bool Initialize(ProcessHelper& memory);
	static bool Update(ProcessHelper& memory);
	static DWORD64 GetLocalPlayerPawn(ProcessHelper& memory);
};

namespace GameVars {
	namespace LocalPlayerPawn {
		inline DWORD64 address{ 0 };
		inline int32_t health{ 0 };
		inline int32_t maxHealth{ 0 };
		inline uint8_t teamNum{ 0 };
		inline uint32_t flags{ 0 };
		inline float position[3]{ 0.0f, 0.0f, 0.0f }; // m_vOldOrigin
	}
}