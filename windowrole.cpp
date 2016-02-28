#include "windowrole.h"
#include <stdexcept>

QString to_QString( const WindowRole& wr) {
	if( wr == WindowRole::AudienceWindow ) {
		return QString::fromUtf8("Audience_Window");
	} else {
		return QString::fromUtf8("Secondary_Window");
	}
}
