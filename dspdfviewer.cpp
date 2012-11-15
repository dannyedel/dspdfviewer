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

#include <QDebug>
#include <stdexcept>

DSPDFViewer::DSPDFViewer(QString filename, bool splitMode): 
	presentationClocksRunning(false),
	readyToRender(false),
	pdfDocument(Poppler::Document::load(filename))
	,
 renderFactory(filename),
 m_pagenumber(0),
 audienceWindow(0,  splitMode? PagePart::LeftHalf : PagePart::FullPage, false),
 secondaryWindow(1, splitMode? PagePart::RightHalf: PagePart::FullPage, true)
{
  qDebug() << "Starting constructor" ;
  audienceWindow.setViewer(this);
  secondaryWindow.setViewer(this);
  
  audienceWindow.showLoadingScreen(0);
  secondaryWindow.showLoadingScreen(0);
  
  if ( ! pdfDocument  || pdfDocument->isLocked() )
  {
    /// FIXME: Error message
    throw std::runtime_error("I was not able to open the PDF document. Sorry.");
  }
  setHighQuality(true);
  
  connect( &renderFactory, SIGNAL(pageRendered(QSharedPointer<RenderedPage>)), &audienceWindow, SLOT(renderedPageIncoming(QSharedPointer<RenderedPage>)));
  connect( &renderFactory, SIGNAL(pageRendered(QSharedPointer<RenderedPage>)), &secondaryWindow, SLOT(renderedPageIncoming(QSharedPointer<RenderedPage>)));
  
  connect( &renderFactory, SIGNAL(thumbnailRendered(QSharedPointer<RenderedPage>)), &audienceWindow, SLOT(renderedThumbnailIncoming(QSharedPointer<RenderedPage>)));
  connect( &renderFactory, SIGNAL(thumbnailRendered(QSharedPointer<RenderedPage>)), &secondaryWindow, SLOT(renderedThumbnailIncoming(QSharedPointer<RenderedPage>)));
  
  renderPage();
  connect( &clockDisplayTimer, SIGNAL(timeout()), &secondaryWindow, SLOT(refreshClocks()));
  secondaryWindow.refreshClocks();
  clockDisplayTimer.setInterval(250);
  clockDisplayTimer.start();
  readyToRender= true;
}



DSPDFViewer::~DSPDFViewer()
{}

void DSPDFViewer::goBackward()
{
  resetSlideClock();
  gotoPage(pageNumber()-1);
}

void DSPDFViewer::goForward()
{
  resetSlideClock();
  gotoPage(pageNumber()+1);
}

unsigned int DSPDFViewer::pageNumber()
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
#if 0
for( unsigned int i=std::max(3u, m_pagenumber)-3; i<m_pagenumber+6; i++)
  {
    if ( ! secondaryWindow.hasThumbnailForPage(i) )
    {
      QSharedPointer<Poppler::Page> page( pdfDocument->page(i) );
      if ( page ) {
	QImage thumbnail = renderForTarget(page, thumbnailSize, false);
	secondaryWindow.addThumbnail(i, thumbnail);
      }
    }
  }
  secondaryWindow.renderThumbnails(m_pagenumber);
#endif /* 0 */

  qDebug() << "Requesting rendering of page " << m_pagenumber;
  audienceWindow.showLoadingScreen(m_pagenumber);
  secondaryWindow.showLoadingScreen(m_pagenumber);
  theFactory()->requestThumbnailRendering(m_pagenumber);
  theFactory()->requestPageRendering(m_pagenumber, audienceWindow);
  theFactory()->requestPageRendering(m_pagenumber, secondaryWindow);
  
  /** Pre-Render next 10 pages **/
  for ( unsigned i=m_pagenumber; i<m_pagenumber+10 && i < numberOfPages() ; i++) {
    theFactory()->requestThumbnailRendering(i);
    theFactory()->requestPageRendering(i, audienceWindow);
    theFactory()->requestPageRendering(i, secondaryWindow);
  }
  
  /** Request previous 3 pages **/
  
  for ( unsigned i= std::max(m_pagenumber,3u)-3; i<m_pagenumber; i++) {
    theFactory()->requestThumbnailRendering(i);
    theFactory()->requestPageRendering(i, audienceWindow);
    theFactory()->requestPageRendering(i, secondaryWindow);
  }
  
}


void DSPDFViewer::gotoPage(unsigned int pageNumber)
{
  if ( m_pagenumber != pageNumber 
      && numberOfPages() > pageNumber )
  {
    m_pagenumber = pageNumber;
    renderPage();
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

void DSPDFViewer::setHighQuality(bool hq)
{
  pdfDocument->setRenderHint(Poppler::Document::Antialiasing, hq);
  pdfDocument->setRenderHint(Poppler::Document::TextAntialiasing, hq);
  pdfDocument->setRenderHint(Poppler::Document::TextHinting, hq);
}

const QSize DSPDFViewer::thumbnailSize = QSize(200, 100);

PdfRenderFactory* DSPDFViewer::theFactory()
{
  return &renderFactory;
}

unsigned int DSPDFViewer::numberOfPages() {
	if ( pdfDocument->numPages() < 0 )
	{
		/* What the... ?! 
		 *
		 * I return zero, so that any kind of loop that counts "for
		 * all pages" will terminate immediately.
		 */
		return 0;
	}
	/* numPages is non-negative and therefore safe to use. */
	return pdfDocument->numPages() ;
}

bool DSPDFViewer::isReadyToRender()
{
  return readyToRender;
}

void DSPDFViewer::goToStartAndResetClocks()
{
  presentationClocksRunning=false;
  gotoPage(0);
}

QString DSPDFViewer::presentationClock()
{
  if ( ! presentationClocksRunning )
    return timeToString(0);
  return timeToString(presentationStart.elapsed());
}

QString DSPDFViewer::wallClock()
{
  return timeToString( QTime::currentTime() );
}

QString DSPDFViewer::slideClock()
{
  if ( ! presentationClocksRunning )
    return timeToString(0);
  return timeToString( slideStart.elapsed() );
}

QString DSPDFViewer::timeToString(QTime time)
{
  return time.toString("HH:mm:ss");
}

QString DSPDFViewer::timeToString(int milliseconds)
{
  int seconds = milliseconds / 1000;
  int minutes = seconds / 60;
  int hours = minutes / 60;
  seconds%=60;
  minutes%=60;
  QString s;
  s.sprintf("%2d:%02d:%02d", hours, minutes, seconds);
  return s;
}

void DSPDFViewer::resetSlideClock()
{
  /* Always resets the slide clock. */
  slideStart.start();
  if ( ! presentationClocksRunning ) {
    /* If this starts a presentation, also reset the presentation clock. */
    presentationStart.start();
    presentationClocksRunning=true;
  }
  /* Refresh display times immediately */
  secondaryWindow.refreshClocks();
  /* and make sure they'll get refreshed a second later aswell. */
  clockDisplayTimer.start();
}


#include "dspdfviewer.moc"
