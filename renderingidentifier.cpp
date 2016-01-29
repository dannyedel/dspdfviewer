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


#include "renderingidentifier.h"
#include <QApplication>
#include <QHash>
#include <sstream>

#if defined ( POPPLER_QT5 )
#include <QDebugStateSaver>
#else
#include <QDebug>
#endif

bool RenderingIdentifier::operator==(const RenderingIdentifier& other) const
{
  return
    thePageNumber == other.thePageNumber
    && thePagePart == other.thePagePart
    && theRequestedPageSize == other.theRequestedPageSize;
}

#if 0
RenderingIdentifier::operator QString() const
{
  QString s( "page%1_%2_size%3x%4" ) ;
  QString partId;
  switch( pagePart() ) {
    case PagePart::LeftHalf:
      partId= "LeftHalf";
      break;
    case PagePart::RightHalf:
      partId="RightHalf";
      break;
    case PagePart::FullPage:
	partId="FullPage";
	break;
  }
  return s.arg(pageNumber()).arg(partId).arg(requestedPageSize().width()).arg(requestedPageSize().height());
}
#endif

unsigned RenderingIdentifier::pageNumber() const
{
  return thePageNumber;
}

PagePart RenderingIdentifier::pagePart() const
{
  return thePagePart;
}

QSize RenderingIdentifier::requestedPageSize() const
{
  return theRequestedPageSize;
}

RenderingIdentifier::RenderingIdentifier(unsigned pagenum, PagePart pagepart, QSize pagesize):
	thePageNumber(pagenum),
	thePagePart(pagepart),
	theRequestedPageSize(pagesize),
	theVersion(1)
{

}


uint qHash(const RenderingIdentifier& ri)
{
  return qHash(ri.pageNumber()) ^ qHash(ri.requestedPageSize().height()) ^ qHash(ri.requestedPageSize().width());
}

QDebug operator << (QDebug d, const RenderingIdentifier& ri) {
#if defined ( POPPLER_QT5 )
	// QDebugStateSaver only exists in Qt5.1
	QDebugStateSaver s(d);
#endif
	std::ostringstream oss;
	oss << ri.pagePart();
	d.nospace() << "[page " << ri.pageNumber() << '/' << oss.str().c_str() << '/' <<
		ri.requestedPageSize().width() << 'x' << ri.requestedPageSize().height() << ']';
	return d;
}
