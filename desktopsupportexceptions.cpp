#include "desktopsupportexceptions.h"

using namespace std;

namespace {
	string const stringify( const OutputHandle& oh) {
		string s("Output ");
		s+=oh.name();
		if ( oh.isPrimary() )
			s+="[primary]";
		return s;
	}
	string const stringify( const OutputList& ol) {
		string s;
		for( const OutputPtr& p: ol ){
			if (p) {
				s+=stringify(*p);
				s+=", ";
			} else {
				s+="NULL PTR, ";
			}
		}
		return s;
	}
}

DesktopSupportException::DesktopSupportException(const std::string& s):
	runtime_error(s) {
}

NoOutputsFound::NoOutputsFound():
	DesktopSupportException("No outputs were detected") {
}

CannotDeterminePrimaryOutput::CannotDeterminePrimaryOutput( const OutputList& ol ):
	DesktopSupportException("Cannot determine primary output: "+stringify(ol) ) {
}

TooManyScreens::TooManyScreens( const OutputList& ol):
	DesktopSupportException("Too many screens: "+stringify(ol)) {
}


