#include "desktopsupportfactory.h"

#include "desktopsupport/generic.h"
#include "debug.h"

using namespace std;

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
