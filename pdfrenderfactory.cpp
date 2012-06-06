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


#include "pdfrenderfactory.h"
#include "renderthread.h"


#include <QMutexLocker>
#include <QThreadPool>


static const QSize ThumbnailSize(200,100);

void PdfRenderFactory::pageThreadFinishedRendering(QSharedPointer<RenderedPage> renderedPage)
{
  {
    QMutexLocker lock(&mutex);
    const RenderingIdentifier ident( renderedPage->getIdentifier() );
    renderedPages.insert(ident, new RenderedPage(*renderedPage));
    currentlyRenderingPages.remove(ident);
  }
  
  emit pageRendered(renderedPage);
}

void PdfRenderFactory::thumbnailThreadFinishedRendering(QSharedPointer<RenderedPage> renderedPage)
{
  {
    QMutexLocker lock(&mutex);
    int pageNumber = renderedPage->getPageNumber();
    renderedThumbnails.insert(pageNumber, new RenderedPage(*renderedPage));
    currentlyRenderingThumbnails.remove(pageNumber);
  }
}

void PdfRenderFactory::initialize()
{
  if ( !m_document || m_document->isLocked() )
    throw QString("Document not readable");
  m_document->setRenderHint(Poppler::Document::Antialiasing, true);
  m_document->setRenderHint(Poppler::Document::TextAntialiasing, true);
  m_document->setRenderHint(Poppler::Document::TextHinting, true);
}


PdfRenderFactory::PdfRenderFactory(QSharedPointer< Poppler::Document > document): QObject(), m_document(document)
{
  initialize();
}

PdfRenderFactory::PdfRenderFactory(QString filename): QObject(), m_document(Poppler::Document::load(filename) )
{
  initialize();
}

void PdfRenderFactory::requestPageRendering(int pageNumber, QSize targetSize, PagePart targetPart)
{
  QMutexLocker lock(&mutex);
  
  RenderingIdentifier r( pageNumber, targetPart, targetSize );
  if ( renderedPages.contains(r) )
  {
    /* Page is ready. Take a copy and lets go. */
    QSharedPointer<RenderedPage> page( new RenderedPage( * renderedPages.object(r) ));
    emit pageRendered(page);
    return;
  }
  /* Page was not in cache. Check if its currently in the render stage. */
  if ( currentlyRenderingPages.contains(r) )
  {
    /* Page is already rendering, so there is nothing to do. */
    return;
  }
  
  
  /* Nobody is working on the page right now. Lets create it. */
  QSharedPointer< Poppler::Page > page( m_document->page(pageNumber) );
  RenderThread* t = new RenderThread( page, r );
  connect( t, SIGNAL(renderingFinished(RenderedPage)), this, SLOT(pageThreadFinishedRendering(RenderedPage)));
  currentlyRenderingPages.insert(r);
  QThreadPool::globalInstance()->start(t);
  
}

void PdfRenderFactory::requestThumbnailRendering(int pageNumber)
{
  QMutexLocker lock(&mutex);
  
  if ( renderedThumbnails.contains(pageNumber) )
  {
    /* Its ready. Take a copy and lets go. */
    QSharedPointer<RenderedPage> thumb( new RenderedPage( * renderedThumbnails.object(pageNumber ) ));
    emit thumbnailRendered(thumb);
    return;
  }
  
  if ( currentlyRenderingThumbnails.contains(pageNumber) )
  {
    /* Its in the rendering process, the signal will be emitted later. Nothing to do. */
    return;
  }
  
  /* We have to render it */
  RenderingIdentifier r(pageNumber, PagePart::FullPage, ThumbnailSize);
  
  QSharedPointer< Poppler::Page > page(m_document->page(pageNumber) );
  RenderThread* t = new RenderThread(page, r);
  connect( t, SIGNAL(renderingFinished(RenderedPage)), this, SLOT(thumbnailThreadFinishedRendering(RenderedPage)));
  currentlyRenderingThumbnails.insert(pageNumber);
  QThreadPool::globalInstance()->start(t);
}


#include "pdfrenderfactory.cpp.moc"

