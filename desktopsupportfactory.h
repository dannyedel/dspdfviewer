#pragma once

#include "desktopsupport.h"

#include <memory>

typedef std::unique_ptr< DesktopSupport > DesktopSupportPtr;

/** Compare two DesktopSupportPtr's by their quality */
inline bool lessQuality (
	const DesktopSupportPtr& lhs,
	const DesktopSupportPtr& rhs
	) {
	return lhs->quality() < rhs->quality();
}

class DesktopSupportFactory {
public:
	/** Initializes all compiled-in modules,
	 * figures out which one is best suited for
	 * the current environment,
	 * and returns the instance.
	 */

	static DesktopSupportPtr getDesktopSupport();
};
