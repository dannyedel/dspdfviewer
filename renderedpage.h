/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2012  <copyright holder> <email>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef RENDEREDPAGE_H
#define RENDEREDPAGE_H
#include <qimage.h>
#include <poppler/qt4/poppler-qt4.h>
#include "pagepart.h"
#include "renderingidentifier.h"

class RenderedPage
{
private:
  QImage theRenderedImage;
  QList< QSharedPointer<Poppler::Link> > theLinks;
  RenderingIdentifier theIdentifier;
public:
  
  RenderedPage(QImage img, QList<QSharedPointer< Poppler::Link >> links, PagePart whichPart, unsigned pageNum);
  RenderedPage(QImage img, QList<QSharedPointer< Poppler::Link >> links, RenderingIdentifier identifier);
  QImage getImage() const;
  QList< QSharedPointer<Poppler::Link> > getLinks() const;
  PagePart getPart() const;
  uint getPageNumber() const;
  RenderingIdentifier getIdentifier() const;
};

Q_DECLARE_METATYPE(QSharedPointer<RenderedPage>)

#endif // RENDEREDPAGE_H
