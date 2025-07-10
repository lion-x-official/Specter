#pragma once

#include <Windows.h>

#include "core/ProcessHelper.hpp"
#include "utils/offsets.hpp"

class Game{
public:
	static bool Initialize(ProcessHelper& memory);
	static DWORD64 GetLocalPlayerPawn(ProcessHelper& memory);
};
