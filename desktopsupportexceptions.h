#pragma once

#include <stdexcept>
#include <string>
#include "desktopsupport.h"

/** Desktop support exceptions */


struct DesktopSupportException: public std::runtime_error {
	DesktopSupportException(const std::string&);
};

struct CannotDeterminePrimaryOutput: public DesktopSupportException {
	CannotDeterminePrimaryOutput(const OutputList& outputs);
};

struct NoOutputsFound: public DesktopSupportException {
	NoOutputsFound();
};

struct OnlyOneOutput: public DesktopSupportException {
	OnlyOneOutput(const OutputHandle& output);
};

struct TooManyScreens: public DesktopSupportException {
	TooManyScreens(const OutputList& outputs);
};

struct FailedToMoveToOutput: public DesktopSupportException {
	FailedToMoveToOutput(const QWidget& window, const OutputHandle& to);
};
