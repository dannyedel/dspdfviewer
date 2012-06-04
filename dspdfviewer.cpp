#include "dspdfviewer.h"

#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>
#include <QApplication>
#include <QDesktopWidget>

DSPDFViewer::DSPDFViewer(QString filename): pdfDocument(
  Poppler::Document::load(filename)
),
 primaryWindow(),
 secondaryWindow()
{
  if ( ! pdfDocument  || pdfDocument->isLocked() )
  {
    /// FIXME: Error message
    throw QString("Shit");
  }
  
  QRect screen0 = QApplication::desktop()->screenGeometry(0);
  primaryWindow.move(screen0.topLeft());
  primaryWindow.showFullScreen();
  
  QRect screen1 = QApplication::desktop()->screenGeometry(1);
  secondaryWindow.move(screen1.topLeft());
  secondaryWindow.showFullScreen();
}

DSPDFViewer::~DSPDFViewer()
{}



#include "dspdfviewer.moc"
