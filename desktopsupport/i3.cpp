#include "i3.h"
#include "../debug.h"
#include <boost/throw_exception.hpp>
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <thread>
#include <chrono>
#include <QApplication>

namespace {
	typedef std::unique_ptr<
		FILE,
		decltype(&pclose)
	> FILEptr;
}

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
		<< "fullscreen disable, "
		<< "move to output " << h.xrandrName;
	cerr << "Trying to run " << command.str() << endl;
	system( command.str().c_str() );
	QApplication::processEvents();
}

void i3DesktopSupport::makeFullscreen(QWidget& w) const {
	ostringstream command;
	command << "i3-msg "
		<< "[id=" << w.winId() << "] "
		<< "fullscreen enable";
	cerr << "Trying to run " << command.str() << endl;
	system( command.str().c_str() );
	QApplication::processEvents();
}


const string i3DesktopSupport::getSocketpath() {
	/** Cache for the socket path */
	static string spath;
	if ( ! spath.empty() ) {
		return spath;
	}
	/** Socketpath not cached */

	/** FIXME: Port this to boost or similar */
	FILEptr fd( popen("i3 --get-socketpath", "r"), pclose );


	return "/run/user/1000/i3/ipc-socket.1468";
}
