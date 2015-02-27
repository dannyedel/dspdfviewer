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
#include <QThread>
#include <QFileSystemWatcher>
#include <QTimer>
#include <qimage.h>
#include <poppler/qt4/poppler-qt4.h>
#include "renderedpage.h"
#include "pdfcacheoption.h"
#include "pdfdocumentreference.h"

/** Priority hint to the render system.
 * Values are ordered in increasing priority.
 */
enum class RenderPriority {
	/* Pre-Render of presenter's screen
	 * (lowest priority)
	 */
	PreRenderSecondary,

	/* Pre-Render for the audience screen */
	PreRenderAudience,

	/* Thumbnails */
	Thumbnail,

	/* Current page, presenter's screen */
	CurrentPageSecondary,

	/* The page currently needed on audience screen
	 * (highest priority)
	 */
	CurrentPageAudience
};


/** Factory for rendered pages
 *
 * This class is responsible for rendering the PDF to images.
 *
 * You create a factory using a filename (if the file cannot be opened this will throw),
 * then you can simply request renderings by supplying the desired size and page part
 * to requestPageRendering.
 *
 * The class uses the global instance of QThreadPool and renders in paralell, meaning it
 * can take advantage of SMP or Multi-Core Systems.
 *
 * NOTE for updating the external file:
 * If you plan on rewriting the displayed file while this application is running,
 * you can cache the entire file to memory. This can be done via constructor parameter.
 *
 * That way, if you create an invalid PDF file (for example you make an error in your latex
 * document), it still has a copy of the old file present.
 * The tradeoff is that this will cost you additional memory for *the entire PDF file*, which
 * can be a lot if you have an image-heavy PDF file.
 *
 * The class will notify you via signals when it detects a file change. Then it will try to re-read
 * the file, and it will notify you if that has succeeded or failed.
 *
 * If the re-reading failed, you can still get pages from cache, and you can keep rendering new ones
 * if you have used the memory cache. However, if you try to render new pages, you will probably
 * experience strange behaviour or the program will crash.
 *
 * If the re-reading went well, the cache will be cleared and new page renders will use the new pdf
 * file.
 *
 *
 * When switching pages, the recommended call order is
 *
 * 1. clearRenderPipeline()
 * 2. setCurrentPage()
 * 3. requestPageRendering() for the current page's views
 * 4. requestPageRendering() for the the thumbnails
 * 5. requestPageRendering() for the pre-render pages
 *
 */
class PdfRenderFactory : public QObject
{
  Q_OBJECT

private:
  PDFDocumentReference documentReference;
  QFileSystemWatcher fileWatcher;
  QTimer fileWatcherRewatchTimer;

  QSet< RenderingIdentifier > currentlyRenderingPages;
  QSet < int > currentlyRenderingThumbnails;

  QCache< RenderingIdentifier, RenderedPage> renderedPages;
  QCache< int, RenderedPage > renderedThumbnails;

  mutable QMutex mutex;

  /** This is a little helper for the cache-clear-function to detect
   * renderings that have been started before, but finished after
   * a cache clearing.
   */
  quint64 currentVersion;

  int numberOfPages_;

private:
  void clearAllCaches();

public:
  PdfRenderFactory( const QString& filename, const PDFCacheOption& cacheSetting );

  /** Request a page rendering.
   *
   * If you use setCurrentPage(), pages within the same priority class (i.e. Thumbnails)
   * will be rendered according to their distance to the current page.
   */
  void requestPageRendering( const RenderingIdentifier& originalIdentifier, const RenderPriority& priorityHint );

  /** Sets the current page.
   * If multiple pages in the same priority class are waiting,
   * those closest to the current page will be rendered first.
   */
  void setCurrentPage(unsigned currentPage);

	/** Returns number of pages */
	int numberOfPages() const;

	/** Clears the rendering pipeline.
	 * Any pages already rendering will not be canceled.
	 *
	 * This is recommended to call when switching pages,
	 * and re-order the pages with their correct priorities.
	 */
	void clearRenderPipeline();

private slots:
  void fileOnDiskChanged(const QString& filename);
  void pageThreadFinishedRendering( QSharedPointer<RenderedPage> renderedPage );

signals:
  void pageRendered( QSharedPointer<RenderedPage> renderedPage);

  void pdfFileChanged();
  void pdfFileRereadSuccesfully();
  void pdfFileRereadFailed();

  public slots:
  void rewatchFile();
};

#endif // PDFRENDERFACTORY_H
