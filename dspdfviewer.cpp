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
 primaryWindow(0),
 secondaryWindow(1),
 m_pagenumber(0)
{
  primaryWindow.setViewer(this);
  secondaryWindow.setViewer(this);
  if ( ! pdfDocument  || pdfDocument->isLocked() )
  {
    /// FIXME: Error message
    throw QString("Shit");
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

void DSPDFViewer::renderPage()
{
  if ( ! currentPage )
  {
    throw QString("Oh crap");
  }
  qDebug() << "Rendering page " << m_pagenumber;
  QSize pagesize = currentPage->pageSize();
  
  /** Render left half on primary display */
  QRect primaryRect = primaryWindow.getTargetWindowSize();
  int primWidth = primaryRect.width();
  int primHeight = primaryRect.height();
  
  QRect secondaryRect = secondaryWindow.getTargetWindowSize();
  int secondWidth = secondaryRect.width();
  int secondHeight = secondaryRect.height();
  
  /** Goal: Render half the page size to primWidth **/
  int halfPageSizePoints = pagesize.width()/2;
  int pageHeightPoints = pagesize.height();
  
  /** DPI value: Pixels per Inch.
   */
  double primaryDPIw = 1.0 * primWidth / ( halfPageSizePoints / 72.0);
  double primaryDPIh = 1.0 * primHeight / ( pageHeightPoints / 72.0);
  double secondaryDPIw = 1.0 * secondWidth / (halfPageSizePoints / 72.0);
  double secondaryDPIh = 1.0 * secondHeight / ( pageHeightPoints / 72.0);
  
  qDebug() << "Computed DPI Primary: " << primaryDPIw << "x" <<primaryDPIh;
  qDebug() << "Computed DPI Secondary: " << secondaryDPIw << "x" << secondaryDPIh;
  
  double primaryDPI = std::min(primaryDPIh,primaryDPIw);
  
  /** Total page width in Pixels **/
  int pageWidthPixelsL = primaryDPI * ( pagesize.width() / 72.0 );
  int pageHeightPixelsL = primaryDPI * ( pagesize.height() / 72.0 );
  
  double secondaryDPI = std::min(secondaryDPIh, secondaryDPIw);
  
  int pageWidthPixelsR = secondaryDPI * ( pagesize.width() / 72.0 );
  int pageHeightPixelsR = secondaryDPI * ( pagesize.height() / 72.0 );
  
  QImage leftImage;
  QImage rightImage;
  
  QImage thumbnail = currentPage->thumbnail();
  
  
  if ( thumbnail.isNull() )
  {
    qDebug() << "No thumbnail available. Rendering one ourselves.";
    QDateTime t1 = QDateTime::currentDateTime();
    thumbnail = currentPage->renderToImage(20.0, 20.0);
    QDateTime t2 = QDateTime::currentDateTime();
    qDebug() << "Thumbnail rendering took " << t1.msecsTo(t2) << " msecs";
    
  }
  
  leftImage = thumbnail.copy(0,0, thumbnail.width()/2, thumbnail.height());
  rightImage = thumbnail.copy(thumbnail.width()/2+1, 0, thumbnail.width()- (thumbnail.width()/2+1), thumbnail.height());
  
  
  QDateTime t = QDateTime::currentDateTime();
  if ( leftImage.isNull() )
  {
    qDebug() << "No thumbnail for left side available.";
    leftImage = currentPage->renderToImage(
      primaryDPI,
      primaryDPI,
      0, 0,
      pageWidthPixelsL/2,
      pageHeightPixelsL);
  }
  primaryWindow.displayImage(leftImage);
  QDateTime render1 = QDateTime::currentDateTime();
  qDebug() << "Left image rendered, took " << t.msecsTo(render1) << " msecs.";
  if ( rightImage.isNull() )
  {
    qDebug() << "No thumbnail for right side available.";
      rightImage = currentPage->renderToImage(
    secondaryDPI,
    secondaryDPI,
    pageWidthPixelsR/2,
    0,
    pageWidthPixelsR-pageWidthPixelsR/2,
    pageHeightPixelsR);
  }
  secondaryWindow.displayImage(rightImage);
  QDateTime render2 = QDateTime::currentDateTime();
  qDebug() << "Right image rendered, took " << render1.msecsTo(render2) << " msecs.";
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
  if ( primaryWindow.getMonitor() == 0 )
  {
    primaryWindow.setMonitor(1);
    secondaryWindow.setMonitor(0);
    renderPage();
  }
  else
  {
    primaryWindow.setMonitor(0);
    secondaryWindow.setMonitor(1);
    renderPage();
  }
}



void DSPDFViewer::exit()
{
  primaryWindow.close();
  secondaryWindow.close();
}

void DSPDFViewer::setHighQuality(bool hq)
{
  pdfDocument->setRenderHint(Poppler::Document::Antialiasing, hq);
  pdfDocument->setRenderHint(Poppler::Document::TextAntialiasing, hq);
  pdfDocument->setRenderHint(Poppler::Document::TextHinting, hq);
}


#include "dspdfviewer.moc"
