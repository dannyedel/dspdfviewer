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
#include "sconnect.h"

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

  sconnect( &renderFactory, SIGNAL(pageRendered(QSharedPointer<RenderedPage>)), &audienceWindow, SLOT(renderedPageIncoming(QSharedPointer<RenderedPage>)));
  sconnect( &renderFactory, SIGNAL(pdfFileRereadSuccesfully()), this, SLOT(renderPage()));

  sconnect( &audienceWindow, SIGNAL(nextPageRequested()), this, SLOT(goForward()));
  sconnect( &audienceWindow, SIGNAL(previousPageRequested()), this, SLOT(goBackward()));
  sconnect( &audienceWindow, SIGNAL(pageRequested(uint)), this, SLOT(gotoPage(uint)));

  sconnect( &audienceWindow, SIGNAL(quitRequested()), this, SLOT(exit()));
  sconnect( &audienceWindow, SIGNAL(rerenderRequested()), this, SLOT(renderPage()));
  sconnect( &audienceWindow, SIGNAL(restartRequested()), this, SLOT(goToStartAndResetClocks()));

  sconnect( &audienceWindow, SIGNAL(screenSwapRequested()), this, SLOT(swapScreens()) );

  sconnect( &audienceWindow, SIGNAL(blankToggleRequested()), this, SLOT(toggleAudienceScreenBlank()));
  sconnect( &audienceWindow, SIGNAL(secondScreenFunctionToggleRequested()), this, SLOT(toggleSecondaryScreenFunction()));

  if ( r.useSecondScreen() )
  {
    DEBUGOUT << "Connecting secondary window";

    secondaryWindow.setPageNumberLimits(0, numberOfPages()-1);

    sconnect( &renderFactory, SIGNAL(pageRendered(QSharedPointer<RenderedPage>)), &secondaryWindow, SLOT(renderedPageIncoming(QSharedPointer<RenderedPage>)));

    sconnect( &secondaryWindow, SIGNAL(nextPageRequested()), this, SLOT(goForward()));
    sconnect( &secondaryWindow, SIGNAL(previousPageRequested()), this, SLOT(goBackward()));
    sconnect( &secondaryWindow, SIGNAL(pageRequested(uint)), this, SLOT(gotoPage(uint)));

    sconnect( &secondaryWindow, SIGNAL(quitRequested()), this, SLOT(exit()));
    sconnect( &secondaryWindow, SIGNAL(rerenderRequested()), this, SLOT(renderPage()));
    sconnect( &secondaryWindow, SIGNAL(restartRequested()), this, SLOT(goToStartAndResetClocks()));

    sconnect( &secondaryWindow, SIGNAL(screenSwapRequested()), this, SLOT(swapScreens()) );

    sconnect( &secondaryWindow, SIGNAL(blankToggleRequested()), this, SLOT(toggleAudienceScreenBlank()));
    sconnect( &secondaryWindow, SIGNAL(secondScreenFunctionToggleRequested()), this, SLOT(toggleSecondaryScreenFunction()));


    sconnect( this, SIGNAL(presentationClockUpdate(QTime)), &secondaryWindow, SLOT(updatePresentationClock(QTime)));
    sconnect( this, SIGNAL(slideClockUpdate(QTime)), &secondaryWindow, SLOT(updateSlideClock(QTime)));
    sconnect( this, SIGNAL(wallClockUpdate(QTime)), &secondaryWindow, SLOT(updateWallClock(QTime)));


  }

  renderPage();

  clockDisplayTimer.setInterval(TIMER_UPDATE_INTERVAL);
  clockDisplayTimer.start();
  sconnect( &clockDisplayTimer, SIGNAL(timeout()), this, SLOT(sendAllClockSignals()));
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


void DSPDFViewer::askFactoryForPage(unsigned pageNumber) {
	/* Request audience window rendering */
	if ( pageNumber == m_pagenumber ) {
		/* If current page, then spawn rendering immediatly */
		theFactory()->requestPageRendering(
			toRenderIdent(
				pageNumber, audienceWindow),
			RenderPriority::CurrentPageAudience);
		/* If we're using two screens, render it aswell */
		if ( runtimeConfiguration.useSecondScreen() ) {
			theFactory()->requestPageRendering(
				toRenderIdent(
					pageNumber, secondaryWindow),
			RenderPriority::CurrentPageSecondary);
		}
	}
	if ( runtimeConfiguration.showThumbnails() ) {
		/** FIXME THUMBNAILS
		theFactory->requestPageRendering(
			toRenderIdent(
				pageNumber, thumbnailBlahFooBar), //FIXME
			RenderPriority::Thumbnail);
		*/
	}
	/* Request low-priority rendering if its not the current page */
	if ( pageNumber != m_pagenumber ) {
		theFactory()->requestPageRendering(
			toRenderIdent(
				pageNumber, audienceWindow),
			RenderPriority::PreRenderAudience);
		/* Second screen aswell */
		if ( runtimeConfiguration.useSecondScreen() ) {
			theFactory()->requestPageRendering(
				toRenderIdent(
					pageNumber, secondaryWindow),
			RenderPriority::PreRenderSecondary);
		}
	}
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

	/* Load current page */
	askFactoryForPage( m_pagenumber );

	/** Pre-Render next pages **/
	for ( unsigned i=m_pagenumber; i<m_pagenumber+runtimeConfiguration.prerenderNextPages() && i < numberOfPages() ; i++) {
		askFactoryForPage(i);
	}

	/** Pre-Render previous pages **/
	for ( unsigned i= std::max(m_pagenumber,runtimeConfiguration.prerenderPreviousPages())-runtimeConfiguration.prerenderPreviousPages();
       i<m_pagenumber; i++) {
		askFactoryForPage(i);
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
