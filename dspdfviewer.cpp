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

#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QApplication>
#include <QDesktopWidget>
#include <qlayout.h>

#include "debug.h"
#include <stdexcept>
#include <boost/numeric/conversion/cast.hpp>

using boost::numeric_cast;

DSPDFViewer::DSPDFViewer(const RuntimeConfiguration& r):
	runtimeConfiguration(r),
	clockDisplayTimer(),
	slideStart(),
	presentationStart(),
	presentationClockRunning(false),
	documentFileWatcher(),
 renderFactory(r.filePathQString(), r.cachePDFToMemory()?PDFCacheOption::keepPDFinMemory:PDFCacheOption::rereadFromDisk ),
 m_pagenumber(0),
 audienceWindow(1,   r.useFullPage()                 ? PagePart::FullPage : PagePart::LeftHalf , false, r, WindowRole::AudienceWindow),
 secondaryWindow(0, (r.useFullPage() | r.duplicate())? PagePart::FullPage : PagePart::RightHalf, true , r, WindowRole::PresenterWindow, r.useSecondScreen())
{
  DEBUGOUT << tr("Starting constructor") ;

  if ( ! r.useSecondScreen() ) {
    secondaryWindow.hide();
  }


  audienceWindow.showLoadingScreen(0);
  secondaryWindow.showLoadingScreen(0);

#if 0 // FIXME Make sure exceptions on startup get handled correctly
  if ( ! pdfDocument  || pdfDocument->isLocked() )
  {
    /// FIXME: Error message
    throw std::runtime_error( tr("I was not able to open the PDF document. Sorry.").toStdString() );
  }
#endif
  DEBUGOUT << tr("Connecting audience window");

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
  sconnect( &audienceWindow, SIGNAL(secondScreenDuplicateRequested()), this, SLOT(toggleSecondaryScreenDuplication()));

  if ( r.useSecondScreen() )
  {
    DEBUGOUT << tr("Connecting secondary window");

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
	sconnect( &secondaryWindow, SIGNAL(secondScreenDuplicateRequested()), this, SLOT(toggleSecondaryScreenDuplication()));


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


void DSPDFViewer::renderPage()
{
  DEBUGOUT << tr("Requesting rendering of page %1").arg(m_pagenumber);
  if ( m_pagenumber >= numberOfPages() ) {
    DEBUGOUT << "Page number out of range, clamping to " << numberOfPages()-1;
    m_pagenumber = numberOfPages()-1;
  }
  audienceWindow.showLoadingScreen(m_pagenumber);
  secondaryWindow.showLoadingScreen(m_pagenumber);
  if ( runtimeConfiguration.showThumbnails() ) {
    theFactory()->requestPageRendering( toThumbnailRenderIdent(m_pagenumber, secondaryWindow), QThread::LowPriority);
  }
  theFactory()->requestPageRendering( toRenderIdent(m_pagenumber, audienceWindow), QThread::HighestPriority);

  if ( runtimeConfiguration.useSecondScreen() ) {
    theFactory()->requestPageRendering( toRenderIdent(m_pagenumber, secondaryWindow), QThread::HighPriority);
  }

  /** Pre-Render next pages **/
  for ( unsigned i=m_pagenumber; i<m_pagenumber+runtimeConfiguration.prerenderNextPages() && i < numberOfPages() ; i++) {
    if ( runtimeConfiguration.showThumbnails() ) {
      theFactory()->requestPageRendering( toThumbnailRenderIdent(i, secondaryWindow), QThread::LowPriority);
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
      theFactory()->requestPageRendering( toThumbnailRenderIdent(i, secondaryWindow), QThread::LowPriority);
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
    qWarning() << tr("Requested page number %1 which is out of range! Ignoring request.").arg(pageNumber);
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
  // Numeric cast includes error handling.
  return numeric_cast<uint>(pages);
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

RenderingIdentifier DSPDFViewer::toThumbnailRenderIdent(unsigned int pageNumber, PDFViewerWindow& window)
{
  QSize newSize = window.getPreviewImageSize();;
  static QSize thumbnailSize;
  if ( thumbnailSize != newSize ) {
    DEBUGOUT << "Thumbnail size changed from" << thumbnailSize << "to" << newSize;
	thumbnailSize=newSize;
	renderPage();
  }
  return RenderingIdentifier( pageNumber, PagePart::FullPage, thumbnailSize);
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

void DSPDFViewer::toggleSecondaryScreenDuplication()
{
  DEBUGOUT << "Swapping second screen duplication";
  switch ( secondaryWindow.getMyPagePart() ) {
    case PagePart::FullPage:
      if (!runtimeConfiguration.useFullPage()) {
        secondaryWindow.setMyPagePart(PagePart::RightHalf);
      }
      break;
    case PagePart::LeftHalf:
    case PagePart::RightHalf:
      secondaryWindow.setMyPagePart(PagePart::FullPage);
      break;
  }
  emit renderPage();
}
