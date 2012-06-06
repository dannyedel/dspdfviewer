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


#include "renderthread.h"
#include "renderutils.h"

RenderThread::RenderThread(QSharedPointer< Poppler::Page > thePage, RenderingIdentifier renderIdentifier):
  QObject(),
  QRunnable(),
  m_page(thePage), renderMe(renderIdentifier)
{
  
}

void RenderThread::run()
{
  QImage renderImage = RenderUtils::renderPagePart(m_page, renderMe.pageSize(), renderMe.pagePart());
  QList< QSharedPointer<Poppler::Link> > links;
  
  for( Poppler::Link* link: m_page->links() )
  {
    QSharedPointer<Poppler::Link> ptrLink(link);
    links.append(ptrLink);
  }
  RenderedPage renderResult( renderImage, links, renderMe );
  
  emit renderingFinished(renderResult);
}

#include "renderthread.cpp.moc"