/*
    dspdfviewer - Dual Screen PDF Viewer for LaTeX-Beamer
    Copyright (C) 2012  Danny Edel <mail@danny-edel.de>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#include "pagepart.h"
#include <string>

namespace {
	const char * const left = "left";
	const char * const right = "right";
	const char * const both = "both";
}

std::ostream& operator << ( std::ostream& out, const PagePart& pagepart ) {
	if ( pagepart == PagePart::LeftHalf )
		return out << left;
	else if ( pagepart == PagePart::RightHalf )
		return out << right;
	else
		return out << both;
}

std::istream& operator >> ( std::istream& in, PagePart& pagepart ) {
	std::string s;
	in >> s;
	if ( s == left )
		pagepart = PagePart::LeftHalf;
	else if ( s == right )
		pagepart = PagePart::RightHalf;
	else if ( s == both )
		pagepart = PagePart::FullPage;
	else
		in.setstate( std::ios::failbit );
	return in;
}
