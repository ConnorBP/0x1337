#pragma once

#include "../interfaces.h"

namespace Trigger {
	void runTrigger(CUserCmd*) noexcept;
	void hitChanceTrigger(CUserCmd* cmd) noexcept;
}