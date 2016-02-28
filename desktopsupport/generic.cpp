#include "generic.h"

#include <QDesktopWidget>
#include <sstream>

using namespace std;

namespace {
	ostream& operator << (ostream& o, const QRect& rect) {
		return o << rect.width() << "x" << rect.height() <<
			"@" << rect.left() << "," << rect.top();
	}
}

string const GenericOutputHandle::name() const {
	ostringstream os;
	os << "GenericOutputHandle - rect " << screenRect;
	return os.str();
}


OutputList GenericDesktopSupport::getOutputs() const {
	OutputList vec;
	QDesktopWidget* desk = QApplication::desktop();
	for( int i=0 ; i < desk->screenCount() ; ++i ) {
		OutputPtr p{ new GenericOutputHandle{
			desk->screenGeometry( i ),
			i == desk->primaryScreen()
			}};
		vec.push_back( move(p) );
	}
	return vec;
}

void GenericDesktopSupport::makeFullscreen(QWidget& window) const {
	if ( window.isFullScreen() )
		return;
	window.showNormal();
	window.showFullScreen();
}

void GenericDesktopSupport::moveWindow(QWidget& window, OutputHandle& outhand) const {
	GenericOutputHandle& out = dynamic_cast<GenericOutputHandle&>(outhand);
	window.move( out.screenRect.topLeft() );
	window.resize( out.screenRect.size() );
}
