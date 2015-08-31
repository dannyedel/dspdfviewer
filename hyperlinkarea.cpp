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


#include "hyperlinkarea.h"
#include <stdexcept>
#include "debug.h"

HyperlinkArea::HyperlinkArea(QLabel* imageLabel, const AdjustedLink& link): QLabel(), targetPage(link.targetPageNumber())
{
  if ( link.linkType() != Poppler::Link::Goto )
    throw WrongLinkType();
  QRect mySize;
  const QPixmap* pixmap = imageLabel->pixmap();
  if ( pixmap == 0 )
    throw /** FIXME Exception **/ std::runtime_error("Tried to construct a HyperlinkArea from an image label without a pixmap");
  
  QRectF sizeWithinImageLabel = link.linkArea();
  
  mySize.setTop(sizeWithinImageLabel.top() * pixmap->height());
  mySize.setLeft(sizeWithinImageLabel.left() * pixmap->width());
  
  mySize.setHeight(
		/** FIXME:
		 * This little abs function causes lots of problems.
		 * clang on linux complains it has to be called std::abs,
		 * appleclang complains that std::abs doesn't exist.
		 *
		 * Replace with some rounding from boost.
		 */
#ifdef __APPLE__
		abs
#else
		std::abs
#endif

		(sizeWithinImageLabel.height() * pixmap->height()));
  mySize.setWidth(sizeWithinImageLabel.width() * pixmap->width()); 
  
  setParent(imageLabel);
  setGeometry(mySize);
  
  /*
   setAutoFillBackground(true);
  QPalette pal = palette();
  pal.setColor(QPalette::Window, QColor(Qt::black) );
  setPalette(pal);
  
  */
  
  // setText( QString("%1 @ +%2,%3").arg(targetPage).arg( link.linkArea().right() ). arg(link.linkArea().top())  );
  
  show();
  
  
  setCursor( Qt::PointingHandCursor );
  
  
  DEBUGOUT << "Added an hyperlink to" << link.targetPageNumber() << "at" << geometry();
}


void HyperlinkArea::mousePressEvent(QMouseEvent* ev)
{
  DEBUGOUT << "Hyperlink clicked" << ev << "Target page" << targetPage;
  
  emit gotoPageRequested(targetPage);
}


#include "hyperlinkarea.moc"
