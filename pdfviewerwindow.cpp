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

PDFViewerWindow::PDFViewerWindow(unsigned int monitor, PagePart myPart, bool showInformationLine, const RuntimeConfiguration& r, bool enabled): 
  QWidget(),
  m_enabled(enabled),
  m_monitor(monitor), myPart(myPart)
{
  if ( ! enabled )
    return;
  setupUi(this);
  if ( !showInformationLine || ! r.showPresenterArea()) {
    /* If the information line is disabled because we're the primary screen,
     * or the user explicitly said so, disable it completely.
     */
    hideInformationLine();
  }
  else {
    /* Enable the information line, but control visibility of the components as requested by the user.
     */
    this->showInformationLine();
    this->wallClock->setVisible(r.showWallClock());
    this->thumbnailArea->setVisible(r.showThumbnails());
    this->slideClock->setVisible(r.showSlideClock());
    this->presentationClock->setVisible(r.showPresentationClock());
  }
  reposition();
}



void PDFViewerWindow::reposition()
{
  if ( ! m_enabled )
    return;
  this->setWindowFlags(windowFlags() & ~Qt::FramelessWindowHint);
  this->showNormal();
  QRect rect = QApplication::desktop()->screenGeometry(getMonitor());
  move(rect.topLeft());
  setFixedSize(rect.size());
  this->setWindowFlags( windowFlags()
  | Qt::FramelessWindowHint
  );
  this->showFullScreen();
  /* Note: The focus should be on the primary window, because at least 
   * Gnome draws the primary window's border onto the secondary.
   * 
   * I dont mind the border on my helper screen, but the
   * audience shouldnt see it.
   */
  if ( !informationLineVisible )
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
    // QWidget::wheelEvent(e);
    
    if ( e->delta() > 0 )
    {
      qDebug() << "Back";
      emit previousPageRequested();
    }
    else{
      qDebug() << "Next";
      emit nextPageRequested();
    }
}

void PDFViewerWindow::keyPressEvent(QKeyEvent* e)
{
    QWidget::keyPressEvent(e);
    
    switch( e->key() )
    {
      case Qt::Key_F12:
      case Qt::Key_S: //Swap
	emit screenSwapRequested();
	break;
      case Qt::Key_Escape:
      case Qt::Key_Q: //quit
	emit quitRequested();
	break;
      case Qt::Key_Space:
      case Qt::Key_Enter:
      case Qt::Key_Return:
      case Qt::Key_PageDown:
      case Qt::Key_Down:
      case Qt::Key_Right:
      case Qt::Key_F: // Forward
      case Qt::Key_N: // Next
	emit nextPageRequested();
	break;
      case Qt::Key_PageUp:
      case Qt::Key_Up:
      case Qt::Key_Left:
      case Qt::Key_Backspace:
      case Qt::Key_B: // Back
      case Qt::Key_P: //Previous
	emit previousPageRequested();
	break;
      case Qt::Key_Home:
      case Qt::Key_H: //Home
	emit restartRequested();
	break;
    }
}


QSize PDFViewerWindow::getTargetImageSize() const
{
  return imageArea->geometry().size();
}

void PDFViewerWindow::mousePressEvent(QMouseEvent* e)
{
    // QWidget::mousePressEvent(e);
    switch (e->button()) {
      case Qt::LeftButton:
	emit nextPageRequested();
	break;
      case Qt::RightButton:
	emit previousPageRequested();
	break;
      default: /* any other button */
	/* do nothing */
	break;
    }
}

void PDFViewerWindow::hideInformationLine()
{
  if ( ! m_enabled )
    return;
  informationLineVisible=false;
  this->bottomArea->hide();
}

bool PDFViewerWindow::isInformationLineVisible() const
{
  return informationLineVisible;
}

void PDFViewerWindow::showInformationLine()
{
  if ( ! m_enabled )
    return;
  informationLineVisible=true;
  this->bottomArea->show();
}

void PDFViewerWindow::addThumbnail(int pageNumber, QImage thumbnail)
{
  if ( pageNumber == currentPageNumber-1)
    previousThumbnail->setPixmap(QPixmap::fromImage(thumbnail));
  else if ( pageNumber == currentPageNumber )
    currentThumbnail -> setPixmap(QPixmap::fromImage(thumbnail));
  else if ( pageNumber == currentPageNumber+1 )
    nextThumbnail->setPixmap(QPixmap::fromImage(thumbnail));
}



void PDFViewerWindow::renderedPageIncoming(QSharedPointer< RenderedPage > renderedPage)
{
  if ( ! m_enabled )
    return;
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
  if ( !m_enabled )
    return;    

  /// FIXME Loading image
  
  this->currentPageNumber = pageNumberToWaitFor;
  this->correntImageRendered = false;  
  this->currentImage = QImage();
  imageLabel->setPixmap(QPixmap());
  imageLabel->setText(QString("Loading page number %1").arg(pageNumberToWaitFor) );
  
  /** Clear Thumbnails, they will come back in soon */
  previousThumbnail->setPixmap( QPixmap() );
  currentThumbnail->setPixmap( QPixmap() );
  nextThumbnail->setPixmap( QPixmap() );
  
}



void PDFViewerWindow::renderedThumbnailIncoming(QSharedPointer< RenderedPage > renderedThumbnail)
{
  if ( !m_enabled )
    return;
  
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
}

PagePart PDFViewerWindow::getMyPagePart() const
{
  return myPart;
}

void PDFViewerWindow::resizeEvent(QResizeEvent* resizeEvent)
{
  if ( !m_enabled )
    return;
  
  QWidget::resizeEvent(resizeEvent);
  qDebug() << "Resize event" << resizeEvent;
  qDebug() << "Resized from" << resizeEvent->oldSize() << "to" << resizeEvent->size() << ", requesting re-render.";
  emit rerenderRequested();
}

QString PDFViewerWindow::timeToString(QTime time) const
{
  return time.toString("HH:mm:ss");
}

QString PDFViewerWindow::timeToString(int milliseconds) const
{
  QTime time;
  time.addMSecs(milliseconds);
  return timeToString(time);
}


void PDFViewerWindow::updatePresentationClock(const QTime& presentationClock)
{
  this->presentationClock->setText( QString("Total\n%1").arg(timeToString(presentationClock)));
}

void PDFViewerWindow::updateSlideClock(const QTime& slideClock)
{
  this->slideClock->setText(timeToString(slideClock) );
}

void PDFViewerWindow::updateWallClock(const QTime& wallClock)
{
  this->wallClock->setText(timeToString(wallClock));
}




#include "pdfviewerwindow.moc"


