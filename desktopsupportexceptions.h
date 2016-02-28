#pragma once

#include <stdexcept>
#include <string>
#include "desktopsupport.h"

/** Desktop support exceptions */

struct DesktopSupportException: public std::runtime_error {
	DesktopSupportException(const std::string&);
};

struct CannotDeterminePrimaryOutput: public DesktopSupportException {
	const OutputList outputs;
	CannotDeterminePrimaryOutput(OutputList&& outputs);
};

struct OnlyOneOutput: public DesktopSupportException {
	const OutputPtr output;
	OnlyOneOutput(OutputPtr&& output);
};

struct TooManyScreens: public DesktopSupportException {
	const OutputList outputs;
	TooManyScreens(OutputList&& outputs);
};

struct FailedToMoveToOutput: public DesktopSupportException {
	FailedToMoveToOutput(const QWidget& window, const OutputHandle& to);
};
