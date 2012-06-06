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

PDFViewerWindow::PDFViewerWindow(unsigned int monitor, PagePart part): QWidget(),
  m_monitor(monitor), myPart(part)
{
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
  reposition();
}



void PDFViewerWindow::reposition()
{
  this->setWindowFlags(windowFlags() & ~Qt::FramelessWindowHint);
  this->showNormal();
  QRect rect = QApplication::desktop()->screenGeometry(getMonitor());
  setFixedSize(rect.size());
  move(rect.topLeft());
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
  imageLabel.setPixmap(QPixmap::fromImage(image));
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
    }
}

void PDFViewerWindow::setViewer(DSPDFViewer* v)
{
  m_dspdfviewer = v;
}

QSize PDFViewerWindow::getTargetImageSize() const
{
  QSize screenSize = QApplication::desktop()->screenGeometry(getMonitor()).size();
  if ( isInformationLineVisible() )
  {
    screenSize.setHeight( screenSize.height() - informationLineLayout->geometry().height() );
  }
  return screenSize;
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
    }
}

void PDFViewerWindow::hideInformationLine()
{
  informationLineVisible=false;
  for( QLabel*l : thumbnailLabels) {
    l->hide();
  }
}

bool PDFViewerWindow::isInformationLineVisible() const
{
  return informationLineVisible;
}

void PDFViewerWindow::showInformationLine()
{
  informationLineVisible=true;
  for( QLabel*l : thumbnailLabels) {
    l->show();
  }
}

void PDFViewerWindow::addThumbnail(int pageNumber, QImage thumbnail)
{
  thumbnails.insert(pageNumber, thumbnail);
}

void PDFViewerWindow::renderThumbnails(int currentPage)
{
  if ( !isInformationLineVisible() )
    return;
  /* clear all current thumbnails */
  for( int i=1; i < informationLineLayout->count()-1; i++ )
  {
    QLabel* p = (QLabel*) informationLineLayout->itemAt(i)->widget();
    p->clear();
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
}
bool PDFViewerWindow::hasThumbnailForPage(int pageNumber) const
{
  return thumbnails.contains(pageNumber);
}



void PDFViewerWindow::renderedPageIncoming(RenderedPage p)
{
  // If we are not waiting for an image, ignore incoming answers.
  if ( correntImageRendered )
    return;
  
  if ( p.getPageNumber() != this->currentPageNumber )
    return; // This page is not for us. Ignore it.
    
  if ( p.getPart() != this->myPart )
    return; // This is not our part
    
  // There is an image incoming that might fit.
  this->imageLabel.setPixmap( QPixmap::fromImage( p.getImage() ) );
  
  // It was even the right size! Yeah!
  if ( p.getIdentifier().requestedPageSize() == getTargetImageSize() ) {
    this->correntImageRendered= true;
  }
}

void PDFViewerWindow::showLoadingScreen(int pageNumberToWaitFor)
{
  /// FIXME Loading image
  
  this->currentPageNumber = pageNumberToWaitFor;
  this->correntImageRendered = false;  
  imageLabel.setText("LOADING");
}






#include "pdfviewerwindow.moc"


