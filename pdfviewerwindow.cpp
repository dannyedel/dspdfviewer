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


#include "pdfviewerwindow.h"
#include "dspdfviewer.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QDebug>

void PDFViewerWindow::setMonitor(const unsigned int monitor)
{
  if ( m_monitor != monitor )
  {
    m_monitor = monitor;
    reposition();
  }
}

unsigned int PDFViewerWindow::getMonitor() const
{
  return m_monitor;
}

PDFViewerWindow::PDFViewerWindow(unsigned int monitor, PagePart part, bool showInformationLine): 
  QWidget(),
  m_dspdfviewer(nullptr),
  m_monitor(monitor), myPart(part)
{
#ifdef USE_OLD_PROGRAMMATIC_LAYOUT
  this->setContentsMargins(0,0,0,0);
  outerlayout = new QVBoxLayout();
  this->setLayout(outerlayout);
  innerlayout = new QHBoxLayout();
  
  informationLineLayout = new QHBoxLayout();
  
  outerlayout->addStretch();
  outerlayout->addLayout(innerlayout);
  outerlayout->addStretch();
  outerlayout->addLayout(informationLineLayout);
  
  innerlayout->addStretch();
  innerlayout->addWidget(&imageLabel);
  innerlayout->addStretch();
  
  outerlayout->setSpacing(0);
  outerlayout->setContentsMargins(0,0,0,0);
  
  innerlayout->setSpacing(0);
  innerlayout->setContentsMargins(0,0,0,0);
  
//  informationLineLayout->setSpacing(10);
  informationLineLayout->addStretch();
  for ( int i=0; i<10; i++)
  {
    if ( i == 3 || i == 4 )
      informationLineLayout->addStretch();
    QLabel* l= new QLabel();
    thumbnailLabels.insert(i, l);
    informationLineLayout->addWidget(l);
  }
  informationLineLayout->addStretch();
  
  hideInformationLine();
  
  QPalette pal = palette();
  pal.setColor(backgroundRole(), Qt::black);
  setPalette(pal);
#else /* New, ui-based layout */
  setupUi(this);
#endif
  if ( !showInformationLine )
    hideInformationLine();
  reposition();
}



void PDFViewerWindow::reposition()
{
  this->setWindowFlags(windowFlags() & ~Qt::FramelessWindowHint);
  this->showNormal();
  QRect rect = QApplication::desktop()->screenGeometry(getMonitor());
  move(rect.topLeft());
  setFixedSize(rect.size());
  this->setWindowFlags( windowFlags()
  | Qt::FramelessWindowHint
  );
  this->showFullScreen();
  this->activateWindow();
//  this->resize( 100, 100 );
 // this->move(rect.topLeft());
  //this->showFullScreen();

}

void PDFViewerWindow::displayImage(QImage image)
{
  currentImage= image;
  imageLabel->setText("");
  imageLabel->resize( image.size() );
  imageLabel->setPixmap(QPixmap::fromImage(image));
  //imageArea->setWidgetResizable(true);
  
  /*
  if ( geometry().size() != getTargetImageSize() )
    reposition();
  */
}


void PDFViewerWindow::wheelEvent(QWheelEvent* e)
{
    QWidget::wheelEvent(e);
    
    if ( ! m_dspdfviewer )
      return;
    
    if ( e->delta() > 0 )
    {
      qDebug() << "Back";
      m_dspdfviewer->goBackward();
    }
    else{
      qDebug() << "Next";
      m_dspdfviewer->goForward();
    }
}

void PDFViewerWindow::keyPressEvent(QKeyEvent* e)
{
    QWidget::keyPressEvent(e);
    
    if ( ! m_dspdfviewer )
      return;
    
    switch( e->key() )
    {
      case Qt::Key_F12:
	m_dspdfviewer->swapScreens();
	break;
      case Qt::Key_Escape:
	m_dspdfviewer->exit();
	break;
      case Qt::Key_Space:
      case Qt::Key_Enter:
      case Qt::Key_Return:
      case Qt::Key_PageDown:
      case Qt::Key_Down:
      case Qt::Key_Right:
	m_dspdfviewer->goForward();
	break;
      case Qt::Key_PageUp:
      case Qt::Key_Up:
      case Qt::Key_Left:
      case Qt::Key_Backspace:
	m_dspdfviewer->goBackward();
	break;
      case Qt::Key_Home:
	m_dspdfviewer->goToStartAndResetClocks();
	break;
    }
}

void PDFViewerWindow::setViewer(DSPDFViewer* v)
{
  m_dspdfviewer = v;
}

QSize PDFViewerWindow::getTargetImageSize() const
{
#ifdef USE_OLD_PROGRAMMATIC_LAYOUT
  QSize screenSize = QApplication::desktop()->screenGeometry(getMonitor()).size();
  if ( isInformationLineVisible() )
  {
    screenSize.setHeight( screenSize.height() - informationLineLayout->geometry().height() );
  }
  return screenSize;
#else
  return imageArea->geometry().size();
#endif
}

void PDFViewerWindow::mousePressEvent(QMouseEvent* e)
{
    QWidget::mousePressEvent(e);
    switch (e->button()) {
      case Qt::LeftButton:
	m_dspdfviewer->goForward();
	break;
      case Qt::RightButton:
	m_dspdfviewer->goBackward();
	break;
      default: /* any other button */
	/* do nothing */
	break;
    }
}

void PDFViewerWindow::hideInformationLine()
{
  informationLineVisible=false;
#ifdef USE_OLD_PROGRAMMATIC_LAYOUT
  for( QLabel*l : thumbnailLabels) {
    l->hide();
  }
#else
  this->bottomArea->hide();
#endif
}

bool PDFViewerWindow::isInformationLineVisible() const
{
  return informationLineVisible;
}

void PDFViewerWindow::showInformationLine()
{
  informationLineVisible=true;
#ifdef USE_OLD_PROGRAMMATIC_LAYOUT
  for( QLabel*l : thumbnailLabels) {
    l->show();
  }
#else
  this->bottomArea->show();
#endif
}

void PDFViewerWindow::addThumbnail(int pageNumber, QImage thumbnail)
{
  if ( hasThumbnailForPage(pageNumber) )
    return;
  thumbnails.insert(pageNumber, thumbnail);
  QLabel*p = new QLabel(this);
  p->setPixmap(QPixmap::fromImage(thumbnail));

  /* Simple way: insert at the end */
  QList<int> keys = thumbnailLabels.keys();
  if ( keys.isEmpty() || keys.last() < pageNumber ) {
    qDebug() << "Adding thumbnail for page" << pageNumber;
    thumbnailLabels.insert(pageNumber, p);
    thumbnailAreaWidget->layout()->addWidget(p);
    return;
  }
  
  /* Insert at correct place */
  thumbnailLabels.insert(pageNumber,p);
  qDebug() << "Re-ordering thumbnails to display page" << pageNumber;
  
  /* Re-Order all thumbnails */
  for( auto wid: thumbnailAreaWidget->layout()->children() ) {
    thumbnailAreaWidget->layout()->removeWidget( dynamic_cast<QWidget*>(wid) );
  }
  for( auto wid: thumbnailLabels ) {
    thumbnailAreaWidget->layout()->addWidget(wid);
  }
#if 0
  if ( thumbnailLabels.keys()
  for( int i=0; i< thumbnailLabels.size(); ++i) {
    if ( thumbnailLabels.
  }
#endif
}

void PDFViewerWindow::renderThumbnails(int currentPage)
{
  if ( !isInformationLineVisible() )
    return;
#ifdef USE_OLD_PROGRAMMATIC_LAYOUT
  /* clear all current thumbnails */
  for( QLabel* p: thumbnailLabels )
  {
    p->setPixmap(QPixmap());
  }
  /* render up to three previous pages */
  for( int page=currentPage-3, i=0; page<currentPage; page++, i++) {
    if ( thumbnails.contains(page) )
    {
      thumbnailLabels[i]->setPixmap( QPixmap::fromImage(thumbnails[page] ));
    }
  }
  /* Render current page */
  if ( thumbnails.contains(currentPage) )
  {
    thumbnailLabels[3]->setPixmap(QPixmap::fromImage(thumbnails[currentPage] ));
  }
  /* render up to 6 next pages */
  for( int page=currentPage+1, i=4; i<10; page++, i++) {
    if ( thumbnails.contains(page) )
    {
      thumbnailLabels[i]->setPixmap(QPixmap::fromImage(thumbnails[page]));
    }
  }
#else
  /** Scroll to thumbnail's label */
  QLabel* l = thumbnailLabels.value(currentPage);
  if ( l )
    thumbnailArea->ensureWidgetVisible(l);
#endif

}
bool PDFViewerWindow::hasThumbnailForPage(int pageNumber) const
{
  return thumbnails.contains(pageNumber);
}



void PDFViewerWindow::renderedPageIncoming(QSharedPointer< RenderedPage > renderedPage)
{
  // If we are not waiting for an image, ignore incoming answers.
  if ( correntImageRendered )
    return;
  
  if ( renderedPage->getPageNumber() != this->currentPageNumber )
    return; // This page is not for us. Ignore it.
    
  if ( renderedPage->getPart() != this->myPart )
    return; // This is not our part
    
  // There is an image incoming that might fit.
  displayImage(renderedPage->getImage());
  
  // It was even the right size! Yeah!
  if ( renderedPage->getIdentifier().requestedPageSize() == getTargetImageSize() ) {
    this->correntImageRendered= true;
  }
}

void PDFViewerWindow::showLoadingScreen(int pageNumberToWaitFor)
{
  /// FIXME Loading image
  
  this->currentPageNumber = pageNumberToWaitFor;
  this->correntImageRendered = false;  
  this->currentImage = QImage();
  imageLabel->setPixmap(QPixmap());
  imageLabel->setText(QString("Loading page number %1").arg(pageNumberToWaitFor) );
}



void PDFViewerWindow::renderedThumbnailIncoming(QSharedPointer< RenderedPage > renderedThumbnail)
{
  /* If a thumbnail for the page we're waiting for is incoming and we have no page at all, its better than nothing */
  if ( renderedThumbnail->getPageNumber() == currentPageNumber
    && currentImage.isNull() )
  {
    QImage myHalf;
    if ( myPart == PagePart::LeftHalf )
    {
      myHalf = renderedThumbnail->getImage().copy(0, 0, renderedThumbnail->getImage().width()/2, renderedThumbnail->getImage().height());
    }
    else if ( myPart == PagePart::RightHalf )
    {
      myHalf = renderedThumbnail->getImage().copy(renderedThumbnail->getImage().width()/2, 0, renderedThumbnail->getImage().width()/2, renderedThumbnail->getImage().height());
    }
    else if ( myPart == PagePart::FullPage )
    {
      myHalf = renderedThumbnail->getImage();
    }
    displayImage(myHalf);
  }
  
  addThumbnail(renderedThumbnail->getPageNumber(), renderedThumbnail->getImage());
  renderThumbnails(currentPageNumber);
}

PagePart PDFViewerWindow::getMyPagePart() const
{
  return myPart;
}

void PDFViewerWindow::resizeEvent(QResizeEvent* resizeEvent)
{
  QWidget::resizeEvent(resizeEvent);
  qDebug() << "Resize event" << resizeEvent;
  if ( m_dspdfviewer ) { 
      qDebug() << "resized from" << resizeEvent->oldSize() << "to" << resizeEvent->size();
      if ( m_dspdfviewer->isReadyToRender() ) {
	qDebug() << "Viewer is ready to render, requesting rendering";
	m_dspdfviewer->renderPage();
      }
  }
  else {
    qDebug() << "m_dspdfviewer not ready, cant render yet";
  }
}

void PDFViewerWindow::refreshClocks()
{
  if ( ! m_dspdfviewer )
    return;
  wallClock->setText( m_dspdfviewer->wallClock() );
  slideClock->setText(m_dspdfviewer->slideClock() );
  presentationClock->setText(m_dspdfviewer->presentationClock());
}


#include "pdfviewerwindow.moc"


