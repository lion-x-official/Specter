#pragma once
#include "core/ProcessHelper.hpp"

class Trigger {
public:
	bool Update(ProcessHelper& memory);

	void setEnabled(bool value) {
		isEnabled = value;
	}

	bool getStatus() const {
		return isEnabled;
	}
private:
	bool isEnabled = false;
};
