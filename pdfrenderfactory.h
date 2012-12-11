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


#ifndef PDFRENDERFACTORY_H
#define PDFRENDERFACTORY_H

#include <QObject>
#include <QMutex>
#include <QCache>
#include <qimage.h>
#include <poppler/qt4/poppler-qt4.h>
#include "renderedpage.h"

class PdfRenderFactory : public QObject
{
  Q_OBJECT
  
private:
  QString documentFilename;
  
  QSet< RenderingIdentifier > currentlyRenderingPages;
  QSet < int > currentlyRenderingThumbnails;
  
  QCache< RenderingIdentifier, RenderedPage> renderedPages;
  QCache< int, RenderedPage > renderedThumbnails;
  
  QMutex mutex;
  
private:
  QSharedPointer<Poppler::Document> fetchDocument();
  
public:
  PdfRenderFactory( QString filename );
  
  void requestPageRendering( const RenderingIdentifier& renderingIdentifier);
  void requestThumbnailRendering( int pageNumber);
  
private slots:
  void pageThreadFinishedRendering( QSharedPointer<RenderedPage> renderedPage );
  void thumbnailThreadFinishedRendering( QSharedPointer<RenderedPage> renderedPage );
  
signals:
  void pageRendered( QSharedPointer<RenderedPage> renderedPage);
  void thumbnailRendered( QSharedPointer<RenderedPage> renderedThumbnail);
  
};

#endif // PDFRENDERFACTORY_H
