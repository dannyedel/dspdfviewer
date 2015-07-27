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


#include "renderedpage.h"

#include "renderingidentifier.h"

RenderingIdentifier RenderedPage::getIdentifier() const
{
  return theIdentifier;
}

QImage RenderedPage::getImage() const
{
  return theRenderedImage;
}

QList< AdjustedLink > RenderedPage::getLinks() const
{
  return theLinks;
}

unsigned RenderedPage::getPageNumber() const
{
  return getIdentifier().pageNumber();
}


PagePart RenderedPage::getPart() const
{
  return getIdentifier().pagePart();
}

RenderedPage::RenderedPage(QImage img, QList< AdjustedLink > links, PagePart whichPart, unsigned pageNum)
: theRenderedImage(img), theLinks(links), theIdentifier(pageNum, whichPart, img.size())
{

}

RenderedPage::RenderedPage(QImage img, QList< AdjustedLink > links, RenderingIdentifier identifier)
: theRenderedImage(img), theLinks(links), theIdentifier(identifier)
{

}




