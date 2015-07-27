/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2012  <copyright holder> <email>

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


#include "adjustedlink.h"

#include <stdexcept>

QSharedPointer< Poppler::Link > AdjustedLink::link() const
{
  return m_link;
}

QRectF AdjustedLink::linkArea() const
{
  QRectF const& orig( link()->linkArea() );
  QRectF retval( link()->linkArea() );
  switch(ri.pagePart()) {
    case PagePart::FullPage:
      break;
    case PagePart::LeftHalf:
      if ( retval.left() > 0.5 ) {
	return QRectF();
      }
      retval.setLeft( orig.left() * 2.0 );
      retval.setWidth( orig.width() * 2.0 );
      break;
    case PagePart::RightHalf:
      if ( retval.right() < 0.5 ) {
	// no part of the rectangle is in our page
	return QRectF();
      }
      retval.setLeft( (orig.left() - 0.5) * 2.0 );
      retval.setWidth( orig.width() * 2.0 );
      break;
  }
  
  if ( retval.height() < 0 ) {
    retval.setHeight( -retval.height() );
    retval.moveTop( retval.top() - retval.height());
  }
  
  return retval;
  
}

Poppler::Link::LinkType AdjustedLink::linkType() const
{
  return link()->linkType();
}


AdjustedLink::AdjustedLink(const RenderingIdentifier& renderIdent, QSharedPointer< Poppler::Link > link)
:  m_link(link), ri(renderIdent)
{
  if ( linkArea().isNull() )
    throw OutsidePage();
}


AdjustedLink::OutsidePage::OutsidePage(): runtime_error("This link is not inside the current page part")
{
}


uint AdjustedLink::targetPageNumber() const
{
  return lgt().destination().pageNumber() -1;
}


Poppler::LinkGoto const& AdjustedLink::lgt() const
{
  return dynamic_cast<Poppler::LinkGoto const&>( *m_link );
}
