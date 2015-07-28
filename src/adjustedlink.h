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


#ifndef ADJUSTEDLINK_H
#define ADJUSTEDLINK_H
#include "renderingidentifier.h"
#include <QSharedPointer>
#include <poppler/qt4/poppler-qt4.h>

#include <stdexcept>

/** Link that is adjusted to a rendered page.
 * This includes failing to construct if it is completely outside the scope.
 */
class AdjustedLink
{
public:
  /** Exception class */
  struct OutsidePage: public std::runtime_error {
    OutsidePage();
  };
  
  /** Exception class */
  struct UnsupportedLinkType{};
  
  AdjustedLink(const RenderingIdentifier& renderIdent, QSharedPointer<Poppler::Link> link);
  
  /** Returns the link area as floating point in the range 0..1 **/
  QRectF linkArea() const;
  
  /** Returns the link area, but scaled to the given QRectangle **/
  QRect linkArea(const QRect& rect) const;
  
  /** Target page number */
  uint targetPageNumber() const;
  
  /** Returns a shared pointer to the Poppler link
   */
  QSharedPointer<Poppler::Link> link() const;
  
  /** Forwarding function for Poppler::Link::LinkType **/
  Poppler::Link::LinkType linkType() const;
  
private:
  QSharedPointer<Poppler::Link> m_link;
  RenderingIdentifier ri;
  
  Poppler::LinkGoto const& lgt() const;
};


#endif // ADJUSTEDLINK_H
