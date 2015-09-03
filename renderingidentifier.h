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


#ifndef RENDERINGIDENTIFIER_H
#define RENDERINGIDENTIFIER_H
#include "pagepart.h"
#include <qglobal.h>
#include <qsize.h>
#include <QString>

class RenderingIdentifier
{
private:
  unsigned thePageNumber;
  PagePart thePagePart;
  QSize theRequestedPageSize;

public:
  // This field needs to be changed after construction, therefore public
  /** The "version" of the file this render started at
   *
   * This will be set and checked by PDFRenderFactory
   */
  quint64 theVersion = 1;

  RenderingIdentifier(unsigned pagenum, PagePart pagepart, QSize requestedPageSize);
  unsigned pageNumber() const;
  PagePart pagePart() const;
  QSize requestedPageSize() const;

  bool operator == (const RenderingIdentifier& other) const;

  /** Cast to a string that is usable as a hash identifier **/
  operator QString() const;
};

#endif // RENDERINGIDENTIFIER_H
