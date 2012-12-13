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


#include "renderthread.h"
#include "renderutils.h"
#include "adjustedlink.h"
#include <QDebug>

RenderThread::RenderThread(QSharedPointer< Poppler::Document > theDocument, RenderingIdentifier renderIdent):
  QObject(),
  QRunnable(),
  m_document(theDocument), renderMe(renderIdent)
{
  m_page = QSharedPointer<Poppler::Page>( m_document->page(renderIdent.pageNumber()) );
}

void RenderThread::run()
{
  qDebug() << "RenderThread for " << renderMe << " started";
  QImage renderImage = RenderUtils::renderPagePart(m_page, renderMe.requestedPageSize(), renderMe.pagePart());
  if ( renderImage.isNull() )
  {
    qDebug() << "RenderThread for " << renderMe << " failed";
    QSharedPointer<RenderingIdentifier> ri( new RenderingIdentifier(renderMe) );
    emit renderingFailed(ri);
    return;
  }
  
  QList< AdjustedLink > links;

  for( Poppler::Link* link: m_page->links() )
  {
    QSharedPointer<Poppler::Link> ptrLink(link);
    AdjustedLink al(renderMe, ptrLink);
    links.append(al);
  }
  QSharedPointer<RenderedPage> renderResult(new RenderedPage( renderImage, links, renderMe ));
  qDebug() << "RenderThread for " << renderMe << " successful, image has size " << renderResult->getImage().size();
  emit renderingFinished(renderResult);
}

#include "renderthread.cpp.moc"