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


#include "dspdfviewer.h"
#include "renderutils.h"
#include "renderingidentifier.h"

#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>
#include <QApplication>
#include <QDesktopWidget>
#include <qlayout.h>

#include "debug.h"
#include <stdexcept>

DSPDFViewer::DSPDFViewer(const RuntimeConfiguration& r):
	runtimeConfiguration(r),
 presentationClockRunning(false),
 renderFactory(r.filePathQString(), r.cachePDFToMemory()?PDFCacheOption::keepPDFinMemory:PDFCacheOption::rereadFromDisk ),
 m_pagenumber(0),
 audienceWindow(1,  r.useFullPage()? PagePart::FullPage : PagePart::LeftHalf , false, r, "Audience_Window"),
 secondaryWindow(0, r.useFullPage()? PagePart::FullPage : PagePart::RightHalf, true,  r, "Secondary_Window", r.useSecondScreen() )
{
  DEBUGOUT << "Starting constructor" ;

  if ( ! r.useSecondScreen() ) {
    secondaryWindow.hide();
  }


  audienceWindow.showLoadingScreen(0);
  secondaryWindow.showLoadingScreen(0);

#if 0 // FIXME Make sure exceptions on startup get handled correctly
  if ( ! pdfDocument  || pdfDocument->isLocked() )
  {
    /// FIXME: Error message
    throw std::runtime_error("I was not able to open the PDF document. Sorry.");
  }
#endif
  DEBUGOUT << "Connecting audience window";

  audienceWindow.setPageNumberLimits(0, numberOfPages()-1);

  connect( &renderFactory, SIGNAL(pageRendered(QSharedPointer<RenderedPage>)), &audienceWindow, SLOT(renderedPageIncoming(QSharedPointer<RenderedPage>)));
  connect( &renderFactory, SIGNAL(thumbnailRendered(QSharedPointer<RenderedPage>)), &audienceWindow, SLOT(renderedThumbnailIncoming(QSharedPointer<RenderedPage>)));
  connect( &renderFactory, SIGNAL(pdfFileRereadSuccesfully()), this, SLOT(renderPage()));

  connect( &audienceWindow, SIGNAL(nextPageRequested()), this, SLOT(goForward()));
  connect( &audienceWindow, SIGNAL(previousPageRequested()), this, SLOT(goBackward()));
  connect( &audienceWindow, SIGNAL(pageRequested(uint)), this, SLOT(gotoPage(uint)));

  connect( &audienceWindow, SIGNAL(quitRequested()), this, SLOT(exit()));
  connect( &audienceWindow, SIGNAL(rerenderRequested()), this, SLOT(renderPage()));
  connect( &audienceWindow, SIGNAL(restartRequested()), this, SLOT(goToStartAndResetClocks()));

  connect( &audienceWindow, SIGNAL(screenSwapRequested()), this, SLOT(swapScreens()) );

  connect( &audienceWindow, SIGNAL(blankToggleRequested()), this, SLOT(toggleAudienceScreenBlank()));
  connect( &audienceWindow, SIGNAL(secondScreenFunctionToggleRequested()), this, SLOT(toggleSecondaryScreenFunction()));

  if ( r.useSecondScreen() )
  {
    DEBUGOUT << "Connecting secondary window";

    secondaryWindow.setPageNumberLimits(0, numberOfPages()-1);

    connect( &renderFactory, SIGNAL(pageRendered(QSharedPointer<RenderedPage>)), &secondaryWindow, SLOT(renderedPageIncoming(QSharedPointer<RenderedPage>)));
    connect( &renderFactory, SIGNAL(thumbnailRendered(QSharedPointer<RenderedPage>)), &secondaryWindow, SLOT(renderedThumbnailIncoming(QSharedPointer<RenderedPage>)));

    connect( &secondaryWindow, SIGNAL(nextPageRequested()), this, SLOT(goForward()));
    connect( &secondaryWindow, SIGNAL(previousPageRequested()), this, SLOT(goBackward()));
    connect( &secondaryWindow, SIGNAL(pageRequested(uint)), this, SLOT(gotoPage(uint)));

    connect( &secondaryWindow, SIGNAL(quitRequested()), this, SLOT(exit()));
    connect( &secondaryWindow, SIGNAL(rerenderRequested()), this, SLOT(renderPage()));
    connect( &secondaryWindow, SIGNAL(restartRequested()), this, SLOT(goToStartAndResetClocks()));

    connect( &secondaryWindow, SIGNAL(screenSwapRequested()), this, SLOT(swapScreens()) );

    connect( &secondaryWindow, SIGNAL(blankToggleRequested()), this, SLOT(toggleAudienceScreenBlank()));
    connect( &secondaryWindow, SIGNAL(secondScreenFunctionToggleRequested()), this, SLOT(toggleSecondaryScreenFunction()));


    connect( this, SIGNAL(presentationClockUpdate(QTime)), &secondaryWindow, SLOT(updatePresentationClock(QTime)));
    connect( this, SIGNAL(slideClockUpdate(QTime)), &secondaryWindow, SLOT(updateSlideClock(QTime)));
    connect( this, SIGNAL(wallClockUpdate(QTime)), &secondaryWindow, SLOT(updateWallClock(QTime)));


  }

  renderPage();

  clockDisplayTimer.setInterval(TIMER_UPDATE_INTERVAL);
  clockDisplayTimer.start();
  connect( &clockDisplayTimer, SIGNAL(timeout()), this, SLOT(sendAllClockSignals()));
  sendAllClockSignals();
}



DSPDFViewer::~DSPDFViewer()
{}

void DSPDFViewer::goBackward()
{
  resetSlideClock();
  if ( pageNumber() > 0 )
    gotoPage(pageNumber()-1);
}

void DSPDFViewer::goForward()
{
  resetSlideClock();
  if ( pageNumber() < numberOfPages()-1 )
    gotoPage(pageNumber()+1);
}

unsigned int DSPDFViewer::pageNumber() const
{
  return m_pagenumber;
}

QImage DSPDFViewer::renderForTarget(QSharedPointer< Poppler::Page > page, QSize targetSize, bool onlyHalf, bool rightHalf)
{
  PagePart part(
    onlyHalf?
      ( rightHalf? PagePart::RightHalf : PagePart::LeftHalf )
      : PagePart::FullPage );
  return RenderUtils::renderPagePart(page, targetSize, part);

}


void DSPDFViewer::renderPage()
{
  DEBUGOUT << "Requesting rendering of page " << m_pagenumber;
  if ( m_pagenumber >= numberOfPages() ) {
    DEBUGOUT << "Page number out of range, clamping to " << numberOfPages()-1;
    m_pagenumber = numberOfPages()-1;
  }
  audienceWindow.showLoadingScreen(m_pagenumber);
  secondaryWindow.showLoadingScreen(m_pagenumber);
  if ( runtimeConfiguration.showThumbnails() ) {
    theFactory()->requestThumbnailRendering(m_pagenumber);
  }
  theFactory()->requestPageRendering( toRenderIdent(m_pagenumber, audienceWindow), QThread::HighestPriority);

  if ( runtimeConfiguration.useSecondScreen() ) {
    theFactory()->requestPageRendering( toRenderIdent(m_pagenumber, secondaryWindow), QThread::HighPriority);
  }

  /** Pre-Render next pages **/
  for ( unsigned i=m_pagenumber; i<m_pagenumber+runtimeConfiguration.prerenderNextPages() && i < numberOfPages() ; i++) {
    if ( runtimeConfiguration.showThumbnails() ) {
      theFactory()->requestThumbnailRendering(i);
    }
    theFactory()->requestPageRendering( toRenderIdent(i, audienceWindow));
    if ( runtimeConfiguration.useSecondScreen() ) {
      theFactory()->requestPageRendering( toRenderIdent(i, secondaryWindow));
    }
  }

  /** Pre-Render previous pages **/

  for ( unsigned i= std::max(m_pagenumber,runtimeConfiguration.prerenderPreviousPages())-runtimeConfiguration.prerenderPreviousPages();
       i<m_pagenumber; i++) {
    if ( runtimeConfiguration.showThumbnails() ) {
      theFactory()->requestThumbnailRendering(i);
    }
    theFactory()->requestPageRendering(toRenderIdent(i, audienceWindow));
    if ( runtimeConfiguration.useSecondScreen() ) {
      theFactory()->requestPageRendering(toRenderIdent(i, secondaryWindow));
    }
  }

}


void DSPDFViewer::gotoPage(unsigned int pageNumber)
{
  if ( m_pagenumber != pageNumber
      && numberOfPages() > pageNumber )
  {
    m_pagenumber = pageNumber;
    renderPage();
  } else {
    qWarning() << "Requested page number" << pageNumber << "which is out of range! Ignoring request.";
  }
}

void DSPDFViewer::swapScreens()
{
  if ( audienceWindow.getMonitor() == 0 )
  {
    audienceWindow.setMonitor(1);
    secondaryWindow.setMonitor(0);
    renderPage();
  }
  else
  {
    audienceWindow.setMonitor(0);
    secondaryWindow.setMonitor(1);
    renderPage();
  }
}



void DSPDFViewer::exit()
{
  audienceWindow.close();
  secondaryWindow.close();
}

const QSize DSPDFViewer::thumbnailSize = QSize(200, 100);

PdfRenderFactory* DSPDFViewer::theFactory()
{
  return &renderFactory;
}

unsigned int DSPDFViewer::numberOfPages() const {
  int pages = renderFactory.numberOfPages();
	if ( pages < 0 )
	{
		/* What the... ?!
		 *
		 * I return zero, so that any kind of loop that counts "for
		 * all pages" will terminate immediately.
		 */
		return 0;
	}
	/* numPages is non-negative and therefore safe to use. */
	return pages;
}

void DSPDFViewer::goToStartAndResetClocks()
{
  presentationClockRunning=false;
  sendAllClockSignals();
  gotoPage(0);
}

QTime DSPDFViewer::presentationClock() const
{
  if ( ! presentationClockRunning )
    return QTime(0,0);
  return timeSince(presentationStart);
}

QTime DSPDFViewer::wallClock() const
{
  return QTime::currentTime();
}

QTime DSPDFViewer::slideClock() const
{
  if ( ! presentationClockRunning )
    return QTime(0,0);
  return timeSince( slideStart );
}

void DSPDFViewer::resetSlideClock()
{
  /* Always resets the slide clock. */
  slideStart.start();
  if ( ! presentationClockRunning ) {
    /* If this starts a presentation, also reset the presentation clock. */
    presentationStart.start();
  }
  /* and make sure they'll get refreshed a second later aswell. */
  slideStart.start();

  presentationClockRunning=true;

  /* Refresh display times immediately */
  sendAllClockSignals();
}

void DSPDFViewer::sendAllClockSignals() const
{
  emit wallClockUpdate(wallClock());
  emit slideClockUpdate(slideClock());
  emit presentationClockUpdate(presentationClock());
}

QTime DSPDFViewer::timeSince(const QTime& startPoint) const
{
  QTime result(0,0);
  result = result.addMSecs(startPoint.elapsed());
  return result;
}

RenderingIdentifier DSPDFViewer::toRenderIdent(unsigned int pageNumber, const PDFViewerWindow& window)
{
  return
  RenderingIdentifier ( pageNumber, window.getMyPagePart(), window.getTargetImageSize());

}

bool DSPDFViewer::isAudienceScreenBlank() const
{
  return audienceWindow.isBlank();
}

void DSPDFViewer::setAudienceScreenBlank()
{
  audienceWindow.setBlank(true);
}

void DSPDFViewer::setAudienceScreenVisible()
{
  audienceWindow.setBlank(false);
}

void DSPDFViewer::toggleAudienceScreenBlank()
{
  if ( isAudienceScreenBlank() ) {
    setAudienceScreenVisible();
  } else {
    setAudienceScreenBlank();
  }
}

void DSPDFViewer::toggleSecondaryScreenFunction()
{
  DEBUGOUT << "Swapping second screen";
  switch ( secondaryWindow.getMyPagePart() ) {
    case PagePart::FullPage:
      // Nothing to do
      break;
    case PagePart::LeftHalf:
      secondaryWindow.setMyPagePart(PagePart::RightHalf);
      break;
    case PagePart::RightHalf:
      secondaryWindow.setMyPagePart(PagePart::LeftHalf);
      break;
  }
  emit renderPage();
}


#include "dspdfviewer.moc"
