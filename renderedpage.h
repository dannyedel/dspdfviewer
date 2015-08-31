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


#ifndef RENDEREDPAGE_H
#define RENDEREDPAGE_H
#include <qimage.h>
#include "poppler-qt.h"
#include "pagepart.h"
#include "renderingidentifier.h"
#include "adjustedlink.h"

class RenderedPage
{
private:
  QImage theRenderedImage;
  QList< AdjustedLink > theLinks;
  RenderingIdentifier theIdentifier;
public:

  RenderedPage(QImage img, QList<AdjustedLink> links, PagePart whichPart, unsigned pageNum);
  RenderedPage(QImage img, QList<AdjustedLink> links, RenderingIdentifier identifier);
  QImage getImage() const;
  QList< AdjustedLink > getLinks() const;
  PagePart getPart() const;
  uint getPageNumber() const;
  RenderingIdentifier getIdentifier() const;
};

Q_DECLARE_METATYPE(QSharedPointer<RenderedPage>)

#endif // RENDEREDPAGE_H
