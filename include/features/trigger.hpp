#pragma once
#include "core/ProcessHelper.hpp"
#include "utils/globals.hpp"


class Trigger {
public:
	bool Update(ProcessHelper& memory);

	void setEnabled(bool value) {
		Globals::triggerEnabled = value;
	}

	bool getStatus() const {
		return Globals::triggerEnabled;
	}
};
