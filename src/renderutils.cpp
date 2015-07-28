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


#include "renderutils.h"

#include "debug.h"
#include <stdexcept>

QImage RenderUtils::renderPagePart(QSharedPointer< Poppler::Page > page, QSize targetSize, PagePart whichPart)
{
  if ( ! page )
  {
    throw std::runtime_error( QString("RenderUtils::renderPagePart called with null page. Target size was %1x%2").
      arg(targetSize.width()).arg(targetSize.height()).toStdString() );
  }

  /* pagesize in points, (72 points is an inch) */
  QSize pagesize = page->pageSize();
  QSize fullsize = pagesize;

  if ( whichPart != PagePart::FullPage )
  {
    /* Only render half the page */
    pagesize.setWidth(pagesize.width()/2);
  }

  /* Calculate DPI for displaying on size */
  /* the 72 comes from converting from points to inches */
  double dpiWidth = 72.0 * targetSize.width() / pagesize.width();
  double dpiHeight = 72.0 * targetSize.height() / pagesize.height();

  /* Take the smaller one, so that the image surely fits on target area */
  double dpi = std::min(dpiWidth, dpiHeight);

  /* Calculate Page Size in pixels */

  QSize fullSizePixels( dpi * fullsize.width() / 72.0,
			dpi * fullsize.height() / 72.0);

  /* Calculate rendered image size */
  QSize imageSizePixels(dpi * pagesize.width() / 72.0,
		  dpi * pagesize.height() / 72.0 );

  /* Calculate x-offset */
  int x = 0;
  if ( whichPart == PagePart::RightHalf ) {
    /* start at an offset of width() pixels to the right */
    x = fullSizePixels.width()/2+1;
  }

  /* render it */
  QImage renderedImage =  page->renderToImage(
    dpi, dpi,
    x, /* x-offset */
    0, /* y-offset */
    imageSizePixels.width(),
    imageSizePixels.height()
		      );

  return renderedImage;
}
