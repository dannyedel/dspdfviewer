#include "desktopsupportfactory.h"
#include "debug.h"

#include "desktopsupport/generic.h"
#include "desktopsupport/win32.h"
#include "desktopsupport/i3.h"

#include <algorithm>
#include <stdexcept>

using namespace std;

namespace {
	/** Compare two DesktopSupportPtr's by their quality */
	inline bool lessQuality (
			const DesktopSupportPtr& lhs,
			const DesktopSupportPtr& rhs
		) {
		return lhs->quality() < rhs->quality();
	}
}

struct NoDesktopSupportImpls: public std::runtime_error {
	NoDesktopSupportImpls():
		runtime_error("No desktop support available. Cannot continue")
	{
	}
};

DesktopSupportPtr DesktopSupportFactory::getDesktopSupport() {
	vector<DesktopSupportPtr> allImpls;

	/** Construct all the implementations here */
	allImpls.emplace_back( new GenericDesktopSupport() );

	/** The Win32 support requires QT5 */
#if defined( POPPLER_QT5 )
	allImpls.emplace_back( new Win32DesktopSupport() );
#endif

	allImpls.emplace_back( new i3DesktopSupport() );

	auto maxElemIt = max_element(
		allImpls.begin(),
		allImpls.end(),
		lessQuality
		);

	DEBUGOUT << "DesktopSupportFactory chose" <<
		(**maxElemIt).name().c_str() <<
		"for the current desktop environment";

	return move( *maxElemIt );
}
