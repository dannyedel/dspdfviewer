#include "desktopsupport.h"
#include "desktopsupportexceptions.h"

namespace {
	bool isPrimary(const OutputPtr& p) {
		return p->isPrimary();
	}
}

OutputPtr DesktopSupport::getPrimary(const DesktopSupportErrorHandling& err) const {
	OutputList vec=getOutputs();

	if ( vec.empty () ) {
		throw NoOutputsFound();
	}

	/** Return (first) primary screen */
	for( OutputPtr& p: vec )
	{
		if ( p->isPrimary() ) {
			return move(p);
		}
	}

	/** No primary screen found!
	 *
	 * If there is not exactly one output left,
	 * we don't know 100% what to do.
	 */

	if ( vec.size() != 1 ) {
		if ( err == DesktopSupportErrorHandling::ThrowOnError )
			throw CannotDeterminePrimaryOutput(vec);
	}

	/** Errors ignored or exactly one screen left. Output it */
	return move( vec.at(0) );
}

OutputPtr DesktopSupport::getSecondary(const DesktopSupportErrorHandling& err) const {
	OutputList vec = getOutputs();
	if ( vec.empty() ) {
		throw NoOutputsFound();
	}

	/** Remove primary outputs from the set */
	vec.erase(
		remove_if( vec.begin(), vec.end(), isPrimary ),
		vec.end()
	);

	/** If there is not exactly one output left,
	 * we have a problem.
	 */
	if ( vec.size() > 1 ) {
		if ( err == DesktopSupportErrorHandling::ThrowOnError )
		{
			throw TooManyScreens(vec);
		}
	}

	/** Return the first non-primary output */
	return move( vec.at(0) );
}

void DesktopSupport::makeFullscreen( QWidget& window ) const {
	if ( isFullscreen(window) )
		return;
	window.showFullScreen();
}

void DesktopSupport::removeFullscreen( QWidget& window ) const {
	if ( ! isFullscreen(window) )
		return;
	window.showNormal();
}

bool DesktopSupport::isFullscreen( QWidget& window ) const {
	return window.isFullScreen();
}
