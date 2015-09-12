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
#include "debug.h"

RenderThread::RenderThread(PDFDocumentReference theDocument, RenderingIdentifier renderIdent):
  QObject(),
  QRunnable(),
  m_page( theDocument.page( renderIdent.pageNumber() ) ),
  renderMe(renderIdent)
{
}

void RenderThread::run()
{
  DEBUGOUT << "RenderThread for " << renderMe.pageNumber() << renderMe.requestedPageSize() << " started";
  QImage renderImage = RenderUtils::renderPagePart(m_page.page, renderMe.requestedPageSize(), renderMe.pagePart());
  if ( renderImage.isNull() )
  {
    qWarning() << "RenderThread for " << renderMe.pageNumber() <<
		renderMe.requestedPageSize().width() << renderMe.requestedPageSize().height() << " failed";
    QSharedPointer<RenderingIdentifier> ri( new RenderingIdentifier(renderMe) );
    emit renderingFailed(ri);
    return;
  }

  QList< AdjustedLink > links;

  for( Poppler::Link* link: m_page.page->links() )
  {
    QSharedPointer<Poppler::Link> ptrLink(link);
    try{
      AdjustedLink al(renderMe, ptrLink);
      links.append(al);
    } catch( AdjustedLink::OutsidePage &) {
      // no-op
    }
  }
  QSharedPointer<RenderedPage> renderResult(new RenderedPage( renderImage, links, renderMe ));
  DEBUGOUT << "RenderThread for " << renderMe.pageNumber() << renderMe.requestedPageSize() << " successful, image has size " << renderResult->getImage().size();
  emit renderingFinished(renderResult);
}
