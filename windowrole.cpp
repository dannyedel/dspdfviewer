#include "windowrole.h"
#include <stdexcept>

QString to_QString( const WindowRole& wr) {
	switch( wr ) {
		case WindowRole::AudienceWindow:
			return QString::fromAscii("Audience_Window");
		case WindowRole::PresenterWindow:
			return QString::fromAscii("Secondary_Window");
	}
	throw std::runtime_error("Control should never get here.");
}
