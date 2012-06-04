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

PDFViewerWindow::PDFViewerWindow(unsigned int monitor): QWidget(),
  m_monitor(monitor)
{
  this->setContentsMargins(0,0,0,0);
  QHBoxLayout* outerlayout = new QHBoxLayout();
  this->setLayout(outerlayout);
  QVBoxLayout* innerlayout = new QVBoxLayout();
  
  outerlayout->addStretch();
  outerlayout->addLayout(innerlayout);
  outerlayout->addStretch();
  
  innerlayout->addStretch();
  innerlayout->addWidget(&label);
  innerlayout->addStretch();
  
  outerlayout->setSpacing(0);
  outerlayout->setContentsMargins(0,0,0,0);
  
  innerlayout->setSpacing(0);
  innerlayout->setContentsMargins(0,0,0,0);
  
  QPalette pal = palette();
  pal.setColor(backgroundRole(), Qt::black);
  setPalette(pal);
  reposition();
}



void PDFViewerWindow::reposition()
{
  QRect rect = QApplication::desktop()->screenGeometry(m_monitor);
  setGeometry(rect);
  this->setWindowFlags( windowFlags() | Qt::FramelessWindowHint);
  this->showFullScreen();
//  this->resize( 100, 100 );
 // this->move(rect.topLeft());
  //this->showFullScreen();

}

void PDFViewerWindow::displayImage(QImage image)
{
  label.setPixmap(QPixmap::fromImage(image));
}


void PDFViewerWindow::mouseDoubleClickEvent(QMouseEvent* e)
{
    QWidget::mouseDoubleClickEvent(e);
    if ( isFullScreen() )
    {
      this->setWindowFlags(windowFlags() & ~Qt::FramelessWindowHint );
      showMaximized();
    } else {
      this->setWindowFlags( windowFlags() | Qt::FramelessWindowHint);
      showFullScreen();
    }
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
    
    if ( e->key() == Qt::Key_F12 )
    {
      m_dspdfviewer->swapScreens();
    }
    else if ( e->key() == Qt::Key_Escape ) {
      m_dspdfviewer->exit();
}
}

void PDFViewerWindow::setViewer(DSPDFViewer* v)
{
  m_dspdfviewer = v;
}





#include "pdfviewerwindow.moc"


