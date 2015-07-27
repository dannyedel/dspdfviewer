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


#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QRunnable>
#include <QObject>
#include "renderedpage.h"
#include "pdfpagereference.h"

class RenderThread: public QObject, public QRunnable
{
  Q_OBJECT
  
private:
  const PDFPageReference m_page;
  RenderingIdentifier renderMe;
  
public:
  RenderThread( PDFDocumentReference theDocument, RenderingIdentifier renderIdent);
  
  void run();
  
  signals:
    void renderingFinished(QSharedPointer<RenderedPage> result);
    void renderingFailed(QSharedPointer<RenderingIdentifier> requestedRender);
};

#endif // RENDERTHREAD_H
