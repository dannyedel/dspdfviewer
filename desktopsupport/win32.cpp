#include "win32.h"

#include <QApplication>
#include <QWindow>
#include <sstream>
#include <boost/throw_exception.hpp>

DesktopEnvironmentHandlerQuality Win32DesktopSupport::quality() const {
#if defined( _WIN32 )
	return DesktopEnvironmentHandlerQuality::OperatingSystemSpecificHandler;
#else
	return DesktopEnvironmentHandlerQuality::NotMyEnvironment;
#endif
}

OutputList Win32DesktopSupport::getOutputs() const {
	/** FIXME: Is this a memory leak?
	 * Who owns the memory pointed to by the pointers?
	 */
	QList<QScreen*> screens = QApplication::screens();
	OutputList outs;
	for( int i=0; i< screens.count() ; ++i ) {
		outs.emplace_back(
			new Win32OutputHandle(i)
		);
	}
	return outs;
}

void Win32DesktopSupport::moveWindow(QWidget& window, OutputHandle& out) const {
	Win32OutputHandle& h=dynamic_cast<Win32OutputHandle&>(out);
	/** Un-fullscreen the window, to be sure we're allowed to move it */
	if ( window.isFullScreen() ) {
		window.showNormal();
	}
	window.windowHandle()->setScreen(
		QApplication::screens().at( h.screenNumber )
	);
}

std::string const Win32OutputHandle::name() const {
	std::ostringstream os;
	os << "Win32 Output [" << screenNumber << "]";
	if ( isPrimary() )
		os << "[PRIMARY]";
	return os.str();
}
