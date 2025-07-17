#pragma once
#include "core/ProcessHelper.hpp"
#include "game/game.hpp"
#include "utils/globals.hpp"


static class Trigger {
public:
	static bool Update(ProcessHelper& memory);

	static void setEnabled(bool value) {
		Globals::triggerEnabled = value;
	}

	static void toggleEnabled() {
		Globals::triggerEnabled = !Globals::triggerEnabled;
	}

	static bool getStatus() {
		return Globals::triggerEnabled;
	}
};
