#include "dspdfviewer.h"

#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>
#include <QApplication>
#include <QDesktopWidget>
#include <qlayout.h>

#include <QDebug>

DSPDFViewer::DSPDFViewer(QString filename): pdfDocument(
  Poppler::Document::load(filename)
),
 audienceWindow(0),
 secondaryWindow(1),
 m_pagenumber(0)
{
  audienceWindow.setViewer(this);
  secondaryWindow.setViewer(this);
  //secondaryWindow.showInformationLine();
  secondaryWindow.hideInformationLine();
  if ( ! pdfDocument  || pdfDocument->isLocked() )
  {
    /// FIXME: Error message
    throw QString("I was not able to open the PDF document. Sorry.");
  }
  setHighQuality(true);
  currentPage.reset( pdfDocument->page(m_pagenumber) );
  renderPage();
}



DSPDFViewer::~DSPDFViewer()
{}

void DSPDFViewer::goBackward()
{
  gotoPage(pageNumber()-1);
}

void DSPDFViewer::goForward()
{
  gotoPage(pageNumber()+1);
}

unsigned int DSPDFViewer::pageNumber()
{
  return m_pagenumber;
}

QImage DSPDFViewer::renderForTarget(std::shared_ptr<Poppler::Page> page, QSize targetSize, bool onlyHalf, bool rightHalf)
{
  qDebug() << "Rendering for target size of " << targetSize;
  if ( ! page )
  {
    throw QString("I dont have a page to render. Thats not good.");
  }
  
  /* pagesize in points, (72 points is an inch) */
  QSize pagesize = page->pageSize(); 
  QSize fullsize = pagesize;
  
  if ( onlyHalf )
  {
    /* Only render half the page */
    pagesize.setWidth(pagesize.width()/2);
  }
  
  /* Calculate DPI for displaying on size */
  /* the 72 comes from converting from points to inches */
  double dpiWidth = 72.0 * targetSize.width() / pagesize.width();
  double dpiHeight = 72.0 * targetSize.height() / pagesize.height();
  
  /* Take the smaller one, so that the image surely fits on target area */
  double dpi = std::min(dpiWidth, dpiHeight);
  
  /* Calculate Page Size in pixels */
  
  QSize fullSizePixels( dpi * fullsize.width() / 72.0,
			dpi * fullsize.height() / 72.0);
  
  /* Calculate rendered image size */
  QSize imageSizePixels(dpi * pagesize.width() / 72.0,
		  dpi * pagesize.height() / 72.0 );
  
  /* Calculate x-offset */
  int x = 0;
  if ( onlyHalf  && rightHalf ) {
    /* start at an offset of width() pixels to the right */
    x = fullSizePixels.width()/2+1;
  }
  
  /* render it */
  QImage renderedImage =  page->renderToImage(
    dpi, dpi,
    x, /* x-offset */
    0, /* y-offset */
    imageSizePixels.width(),
    imageSizePixels.height()
		      );
  
  qDebug() << "Rendered an image of size " << renderedImage.size();
  return renderedImage;
}


void DSPDFViewer::renderPage()
{
  if ( ! currentPage )
  {
    throw QString("Oh crap");
  }
  if (0) {
  for( unsigned int i=std::max(3u, m_pagenumber)-3; i<m_pagenumber+6; i++)
  {
    if ( ! secondaryWindow.hasThumbnailForPage(i) )
    {
      std::shared_ptr<Poppler::Page> page( pdfDocument->page(i) );
      if ( page ) {
	QImage thumbnail = renderForTarget(page, thumbnailSize, false);
	secondaryWindow.addThumbnail(i, thumbnail);
      }
    }
  }
  secondaryWindow.renderThumbnails(m_pagenumber);
  }
  qDebug() << "Rendering page " << m_pagenumber;
  QDateTime t = QDateTime::currentDateTime();
  QImage leftImage = renderForTarget(currentPage, audienceWindow.getTargetImageSize(), true, false);
  QDateTime render1 = QDateTime::currentDateTime();
  qDebug() << "Left image rendered, took " << t.msecsTo(render1) << " msecs.";
  QImage rightImage = renderForTarget(currentPage, secondaryWindow.getTargetImageSize(), true, true);
  QDateTime render2 = QDateTime::currentDateTime();
  qDebug() << "Right image rendered, took " << render1.msecsTo(render2) << " msecs.";
  audienceWindow.displayImage(leftImage);
  secondaryWindow.displayImage(rightImage);
  
}


void DSPDFViewer::gotoPage(unsigned int pageNumber)
{
  if ( m_pagenumber != pageNumber 
      && pdfDocument->numPages() > pageNumber )
  {
    currentPage.reset(pdfDocument->page(pageNumber));
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

#include "dspdfviewer.moc"
