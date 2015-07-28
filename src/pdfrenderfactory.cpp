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


#include "pdfrenderfactory.h"
#include "renderthread.h"

#include <QMutexLocker>
#include <QThreadPool>
#include <stdexcept>
#include "debug.h"


static const QSize ThumbnailSize(200,100);

void PdfRenderFactory::pageThreadFinishedRendering(QSharedPointer<RenderedPage> renderedPage)
{
  {
    QMutexLocker lock(&mutex);
    const RenderingIdentifier ident( renderedPage->getIdentifier() );
    // Ignore this incoming rendering if it was from an old version
    if ( ident.theVersion != currentVersion )
      return;

    renderedPages.insert(ident, new RenderedPage(*renderedPage));
    currentlyRenderingPages.remove(ident);
  }

  emit pageRendered(renderedPage);
}

void PdfRenderFactory::thumbnailThreadFinishedRendering(QSharedPointer<RenderedPage> renderedPage)
{
  {
    QMutexLocker lock(&mutex);
    // Ignore this incoming rendering if it was from an old version
    if (renderedPage->getIdentifier().theVersion != currentVersion )
      return;

    int pageNumber = renderedPage->getPageNumber();
    renderedThumbnails.insert(pageNumber, new RenderedPage(*renderedPage));
    currentlyRenderingThumbnails.remove(pageNumber);
  }

  emit thumbnailRendered(renderedPage);
}

void PdfRenderFactory::rewatchFile()
{
  if ( ! fileWatcher.files().contains( documentReference.filename() ) ) {
    fileWatcher.addPath( documentReference.filename() );
    // Check if it has been added (i.e. if it exists)
    if ( fileWatcher.files().contains( documentReference.filename() ) ) {
      // The file was created in the meantime. FileWatcher does not report this as a "change",
      // So we have to check it manually.
      fileOnDiskChanged(documentReference.filename());
    }
  }
}



PdfRenderFactory::PdfRenderFactory(const QString& filename, const PDFCacheOption& cacheSetting): QObject(), documentReference(filename, cacheSetting), currentVersion(0),

  // Attempt to read the document to get the number of pages within.
  // This will throw an error if the document is unreadable.
  numberOfPages_(documentReference.popplerDocument()->numPages())
{

  rewatchFile();

  // register the on-change function
  connect(&fileWatcher, SIGNAL(fileChanged(QString)), this, SLOT(fileOnDiskChanged(QString)));

  // Make sure it re-watches the file
  fileWatcherRewatchTimer.setInterval(1000);
  connect(&fileWatcherRewatchTimer, SIGNAL(timeout()), this, SLOT(rewatchFile()));
  fileWatcherRewatchTimer.start();
}

void PdfRenderFactory::requestPageRendering(const RenderingIdentifier& originalIdentifier, QThread::Priority priority)
{
  QMutexLocker lock(&mutex);

  RenderingIdentifier renderingIdentifier(originalIdentifier);

  renderingIdentifier.theVersion = currentVersion;

  if ( renderedPages.contains(renderingIdentifier) )
  {
    /* Page is ready. Take a copy and lets go. */
    QSharedPointer<RenderedPage> page( new RenderedPage( * renderedPages.object(renderingIdentifier) ));
    emit pageRendered(page);
    return;
  }
  /* Page was not in cache. Check if its currently in the render stage. */
  if ( currentlyRenderingPages.contains(renderingIdentifier) )
  {
    /* Page is already rendering, so there is nothing to do. */
    return;
  }
  /* Nobody is working on the page right now. Lets create it. */

  RenderThread* t = new RenderThread( documentReference, renderingIdentifier );
  connect(t, SIGNAL(renderingFinished(QSharedPointer<RenderedPage>)), this, SLOT(pageThreadFinishedRendering(QSharedPointer<RenderedPage>)));
  currentlyRenderingPages.insert(renderingIdentifier);
  QThreadPool::globalInstance()->start(t, priority);

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
  r.theVersion = currentVersion;

  RenderThread* t = new RenderThread(documentReference, r);
  connect( t, SIGNAL(renderingFinished(QSharedPointer<RenderedPage>)), this, SLOT(thumbnailThreadFinishedRendering(QSharedPointer<RenderedPage>)));
  currentlyRenderingThumbnails.insert(pageNumber);
  QThreadPool::globalInstance()->start(t, QThread::Priority::LowestPriority);
}

void PdfRenderFactory::fileOnDiskChanged(const QString& filename)
{
  DEBUGOUT << "File" << filename << "has changed on disk";

  if ( filename != documentReference.filename() ) {
    DEBUGOUT << "Ignoring that file.";
    return;
  }

  // Add path back in case it was modified via "move temporary onto filename",
  // which filewatcher treats as a remove and stops watching

  try {
    emit pdfFileChanged();

    {

      // Lock mutex
      QMutexLocker locker(&mutex);

      // Create a new File Reference
      PDFDocumentReference newDoc(filename, documentReference.cacheOption());

      if ( documentReference.cacheOption() == PDFCacheOption::keepPDFinMemory ) {
	// If we keep them in memory, a byte-by-byte compare should be resonably fast.
	// If they are *identical*, we can skip the reloading.
	
	if( documentReference == newDoc ) {
	  DEBUGOUT << "The new document compares identical to the old one, not doing anything.";
	  return;
	}
      }

      // Verify poppler can read this
      newDoc.popplerDocument();

      // replace the current reference with the new one
      documentReference = newDoc;

      numberOfPages_ = documentReference.popplerDocument()->numPages();

      // clear the page cache
      clearAllCaches();
    }

    emit pdfFileRereadSuccesfully();
  } catch( std::runtime_error& e) {
    DEBUGOUT << "Unable to read the new reference. keeping the old one.";
    emit pdfFileRereadFailed();
  }
}

void PdfRenderFactory::clearAllCaches()
{

  // Increment version, so that incoming "old" renders will get ignored
  /// TODO: Send a termination signal to these lingering threads

  ++currentVersion;

  // No renders of the current version are taking place, incoming old renders
  // will be ignored.
  currentlyRenderingPages.clear();
  currentlyRenderingThumbnails.clear();

  // Remove the caches. Since we use explicit copy semantics, its safe to empty
  // these.
  renderedPages.clear();
  renderedThumbnails.clear();

}

int PdfRenderFactory::numberOfPages() const
{
  QMutexLocker lock(&mutex);
  return numberOfPages_;
}


#include "pdfrenderfactory.cpp.moc"

