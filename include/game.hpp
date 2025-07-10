#pragma once

#include <Windows.h>

#include "core/ProcessHelper.hpp"
#include "utils/offsets.hpp"

class Game{
public:
	namespace Vars {
		namespace LocalPlayerPawn {
			inline int32_t health{ 0 };
			inline int32_t maxHealth{ 0 };
			inline uint8_t teamNum{ 0 };
			inline uint32_t flags{ 0 };
		}
	}
	static bool Initialize(ProcessHelper& memory);
	static DWORD64 GetLocalPlayerPawn(ProcessHelper& memory);
};