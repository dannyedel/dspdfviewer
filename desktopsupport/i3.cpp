#include "i3.h"
#include "../debug.h"
#include <sstream>
#include <iostream>
#include <QApplication>

using namespace std;

DesktopEnvironmentHandlerQuality i3DesktopSupport::quality() const {
	/** Check return code of i3 --get-socketpath
	 * to check verify whether we are on i3
	 */
	DEBUGOUT << "Checking if we are on i3...";
	if ( ! getSocketpath().empty() ) {
		return DesktopEnvironmentHandlerQuality::
			DesktopEnvironmentSpecificHandler;
	} else {
		return DesktopEnvironmentHandlerQuality::
			NotMyEnvironment;
	}
}

i3OutputHandle::i3OutputHandle(const string& name, bool prim):
	xrandrName(name), primary(prim)
{
}

const std::string i3OutputHandle::name() const {
	return xrandrName;
}

OutputList i3DesktopSupport::getOutputs() const {
	/** FIXME: Dummy code */
	/** FIXME: Parse output of i3-msg --get-outputs with a JSON parser */
	OutputList list;
	list.emplace_back( new i3OutputHandle("DVI-I-1", true) );
	list.emplace_back( new i3OutputHandle("VGA-0", false) );
	return list;
}

void i3DesktopSupport::moveWindow(QWidget& w, OutputHandle& hnd) const {
	i3OutputHandle& h = dynamic_cast<i3OutputHandle&>( hnd );
	ostringstream command;
	command << "i3-msg " 
		<< "[id=" << w.winId() << "] "
//		<< "fullscreen disable, "
		<< "move to output " << h.xrandrName;
	cerr << "Trying to run " << command.str() << endl;
	system( command.str().c_str() );
	QApplication::processEvents();
}

void i3DesktopSupport::makeFullscreen(QWidget& w) const {
	if ( w.isFullScreen() )
		return;
	ostringstream command;
	command << "i3-msg "
		<< "[id=" << w.winId() << "] "
		<< "fullscreen enable";
	cerr << "Trying to run " << command.str() << endl;
	system( command.str().c_str() );
	QApplication::processEvents();
}

void i3DesktopSupport::removeFullscreen(QWidget& w) const {
	if ( ! w.isFullScreen() )
		return;
	ostringstream command;
	command << "i3-msg "
		<< "[id=" << w.winId() << "] "
		<< "fullscreen disable";
	cerr << "Trying to run " << command.str() << endl;
	system( command.str().c_str() );
	QApplication::processEvents();
}


const string i3DesktopSupport::getSocketpath() {
	/** FIXME: Actually check the return STRING, not just the retval */
	int ret = system("i3 --get-socketpath");

	if ( ret == 0 ) {
		return "OK";
	}

	return string();
}
