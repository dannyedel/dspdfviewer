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


#include "pdfviewerwindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include "debug.h"
#include <QInputDialog>
#include <QMessageBox>
#include "sconnect.h"
#include <cstdlib>
#include <boost/numeric/conversion/cast.hpp>
#include "ui_keybindings.h"

using boost::numeric_cast;

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

PDFViewerWindow::PDFViewerWindow(unsigned int monitor, PagePart pagePart, bool showInformationLine, const RuntimeConfiguration& r, const WindowRole& wr, bool enabled):
  QWidget(),
  ui(),
  m_enabled(enabled),
  m_monitor(monitor),
  currentImage(),
  blank(false),
  informationLineVisible(false),
  currentPageNumber(0),
  minimumPageNumber(0),
  maximumPageNumber(65535),
  correctImageRendered(false),
  myPart(pagePart),
  windowRole(wr),
  runtimeConfiguration(r),
  linkAreas()
{
  if ( ! enabled )
    return;
  ui.setupUi(this);
  unsigned mainImageHeight=100-r.bottomPaneHeight();
  ui.verticalLayout->setStretch(0, numeric_cast<int>(mainImageHeight) );
  ui.verticalLayout->setStretch(1, numeric_cast<int>(r.bottomPaneHeight()) );
  setWindowRole(to_QString(wr));
  /*: User visible Window Title Line */
	if ( windowRole == WindowRole::AudienceWindow ) {
		setWindowTitle(tr("DS PDF Viewer - Audience Window"));
	} else {
		setWindowTitle(tr("DS PDF Viewer - Secondary Window"));
	}
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
    ui.wallClock->setVisible(r.showWallClock());
    ui.thumbnailArea->setVisible(r.showThumbnails());
    ui.slideClock->setVisible(r.showSlideClock());
    ui.presentationClock->setVisible(r.showPresentationClock());
  }

  reposition(); // This will fullscreen on its own
}



void PDFViewerWindow::reposition()
{
  if ( ! m_enabled )
    return;
  this->setWindowFlags(windowFlags() & ~Qt::FramelessWindowHint);
  this->showNormal();
  QRect rect = QApplication::desktop()->screenGeometry( numeric_cast<int>(getMonitor()) );
  move(rect.topLeft());
  resize( rect.size() );
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
  ui.imageLabel->setText( QString() );
  ui.imageLabel->resize( image.size() );
  if ( blank ) {
    // If we're supposed to display a blank image, leave it at this state.
    return;
  }
  currentImage= image;
  ui.imageLabel->setPixmap(QPixmap::fromImage(image));
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
      DEBUGOUT << "Back";
      emit previousPageRequested();
    }
    else{
      DEBUGOUT << "Next";
      emit nextPageRequested();
    }
}

void PDFViewerWindow::keyPressEvent(QKeyEvent* e)
{
    QWidget::keyPressEvent(e);

    switch( e->key() )
    {
      case Qt::Key_F1:
      case Qt::Key_Question: // Help
	keybindingsPopup();
	break;
      case Qt::Key_G:
	changePageNumberDialog();
	break;
      case Qt::Key_F12:
      case Qt::Key_S: //Swap
	emit screenSwapRequested();
	break;
      case Qt::Key_Escape:
      case Qt::Key_Q: //quit
	emit quitRequested();
	break;
      case Qt::Key_T:
	emit secondScreenFunctionToggleRequested();
	break;
      case Qt::Key_D:
	emit secondScreenDuplicateRequested();
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
      case Qt::Key_P: //Previous
	emit previousPageRequested();
	break;
      case Qt::Key_B:
      case Qt::Key_Period:
	emit blankToggleRequested();
	break;
      case Qt::Key_Home:
      case Qt::Key_H: //Home
	emit restartRequested();
	break;
    }
}


QSize PDFViewerWindow::getTargetImageSize() const
{
  return ui.imageArea->geometry().size();
}

QSize PDFViewerWindow::getPreviewImageSize()
{
  QSize completeThumbnailArea = ui.thumbnailArea->frameRect().size();
  DEBUGOUT << "Space for all thumbnails:" << completeThumbnailArea;
  /** FIXME Work needed:
   * since this space must fit three images, we divide horizontal size by three
   */
  QSize thirdThumbnailArea ( completeThumbnailArea.width()/3, completeThumbnailArea.height());
  static QSize lastThumbnailSize = thirdThumbnailArea;
  if ( lastThumbnailSize != thirdThumbnailArea ) {
    lastThumbnailSize=thirdThumbnailArea;
    emit rerenderRequested();
  }
  DEBUGOUT << "Space for one thumbnail:" << thirdThumbnailArea;
  
  return thirdThumbnailArea;
}


void PDFViewerWindow::mousePressEvent(QMouseEvent* e)
{
    // QWidget::mousePressEvent(e);
	if ( e->button() == Qt::LeftButton ) {
		emit nextPageRequested();
	} else if ( e->button() == Qt::RightButton ) {
		emit previousPageRequested();
	}
	// Ignore other buttons.
}

void PDFViewerWindow::hideInformationLine()
{
  if ( ! m_enabled )
    return;
  informationLineVisible=false;
  this->ui.bottomArea->hide();
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
  this->ui.bottomArea->show();
}

void PDFViewerWindow::addThumbnail(uint pageNumber, QImage thumbnail)
{
  if ( pageNumber == currentPageNumber-1)
    ui.previousThumbnail->setPixmap(QPixmap::fromImage(thumbnail));
  else if ( pageNumber == currentPageNumber )
    ui.currentThumbnail -> setPixmap(QPixmap::fromImage(thumbnail));
  else if ( pageNumber == currentPageNumber+1 )
    ui.nextThumbnail->setPixmap(QPixmap::fromImage(thumbnail));
}



void PDFViewerWindow::renderedPageIncoming(QSharedPointer< RenderedPage > renderedPage)
{
  if ( ! m_enabled )
    return;

  // If we're blank, don't do anything with incoming renders.
  // Un-blanking will request a rerender.
  if ( blank )
    return;


  // It might be a thumbnail. If we're waiting for one, check if it would fit.
  if ( isInformationLineVisible()
    && renderedPage->getPart() == PagePart::FullPage
    && renderedPage->getIdentifier().requestedPageSize() == this->getPreviewImageSize() ) {
    this->addThumbnail(renderedPage->getPageNumber(), renderedPage->getImage());
  }
  

  // If we are not waiting for an image, ignore incoming answers.
  if ( correctImageRendered )
    return;

  if ( renderedPage->getPageNumber() != this->currentPageNumber )
    return; // This page is not for us. Ignore it.

  if ( renderedPage->getPart() != this->myPart )
    return; // This is not our part

  // There is an image incoming that might fit.
  displayImage(renderedPage->getImage());

  // It was even the right size! Yeah!
  if ( renderedPage->getIdentifier().requestedPageSize() == getTargetImageSize() ) {
    if ( this->runtimeConfiguration.hyperlinkSupport() ) {
      this->parseLinks(renderedPage->getLinks());
    }
    this->correctImageRendered= true;
  }
}

void PDFViewerWindow::showLoadingScreen(uint pageNumberToWaitFor)
{
  if ( !m_enabled )
    return;
  // If we're blanked, don't render anything.
  if ( blank )
    return;


  /// FIXME Loading image

  this->currentPageNumber = pageNumberToWaitFor;
  this->correctImageRendered = false;
  this->currentImage = QImage();
  ui.imageLabel->setPixmap(QPixmap());
  ui.imageLabel->setText(tr("Loading page number %1").arg(pageNumberToWaitFor) );

  /** Clear Thumbnails, they will come back in soon */
  ui.previousThumbnail->setPixmap( QPixmap() );
  ui.currentThumbnail->setPixmap( QPixmap() );
  ui.nextThumbnail->setPixmap( QPixmap() );

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
  DEBUGOUT << "Resize event" << resizeEvent;
  DEBUGOUT << "Resized from" << resizeEvent->oldSize() << "to" << resizeEvent->size() << ", requesting re-render.";
	static bool i3shellcode_executed = false;
	if (
		windowRole == WindowRole::AudienceWindow &&
		runtimeConfiguration.i3workaround() &&
		resizeEvent->spontaneous() &&
		// i3 generates a spontaneous resize.
		! i3shellcode_executed
		// Make sure to do this only once
		) {
		QApplication::flush(); // Make sure the window has been painted
		// This is the second screen. It has now been created.
		// so we should call the i3 shellcode now
		const std::string shellcode = runtimeConfiguration.i3workaround_shellcode();
		DEBUGOUT << "Running i3 workaround shellcode" << shellcode.c_str();
		int rc = std::system( shellcode.c_str() );
		DEBUGOUT << "Return code of i3-workaround was" << rc ;
		i3shellcode_executed=true;
	}
  emit rerenderRequested();
}

QString PDFViewerWindow::timeToString(const QTime & time) const
{
  return time.toString( tr("HH:mm:ss", "This is used by QTime::toString. See its documentation before changing this.") );
}

QString PDFViewerWindow::timeToString(int milliseconds) const
{
  return timeToString(QTime(0,0).addMSecs(milliseconds));
}


void PDFViewerWindow::updatePresentationClock(const QTime& presentationClock)
{
  ui.presentationClock->setText( QCoreApplication::tr("Form", "Total\n%1").arg(timeToString(presentationClock)));
}

void PDFViewerWindow::updateSlideClock(const QTime& slideClock)
{
  ui.slideClock->setText(timeToString(slideClock) );
}

void PDFViewerWindow::updateWallClock(const QTime& wallClock)
{
  ui.wallClock->setText(timeToString(wallClock));
}

void PDFViewerWindow::keybindingsPopup()
{
	Ui::KeybindingsDialog ui;
	QDialog popup;
	ui.setupUi(&popup);
	popup.exec();
}

void PDFViewerWindow::changePageNumberDialog()
{
  bool ok;
  /* While PDF counts zero-based, users probably think that the first
   * page is called "1".
   */
  uint displayMinNumber = minimumPageNumber+1;
  uint displayMaxNumber = maximumPageNumber+1;
  uint displayCurNumber = currentPageNumber+1;
  int targetPageNumber = QInputDialog::getInt(this,
	/* Window Caption */ tr("Select page"),
	/* Input field caption */
	tr("Jump to page number (%1-%2):").arg(displayMinNumber).arg(displayMaxNumber),
	/* Starting number. */
	numeric_cast<int>(displayCurNumber),
	/* minimum value */
	numeric_cast<int>(displayMinNumber),
	/* maximum value */
	numeric_cast<int>(displayMaxNumber),
	/* Step */
	1,
	/* Did the user accept? */
	&ok);
  targetPageNumber-=1; // Convert back to zero-based numbering scheme
  if ( ok )
  {
    emit pageRequested(numeric_cast<uint>(targetPageNumber));
  }
}

void PDFViewerWindow::setPageNumberLimits(uint minPageNumber, uint maxPageNumber)
{
  this->minimumPageNumber = minPageNumber;
  this->maximumPageNumber = maxPageNumber;
}

void PDFViewerWindow::setBlank(const bool newBlank)
{
  if ( this->blank == newBlank)
    return;
  /* State changes. request re-render */
  this->blank = newBlank;
  DEBUGOUT << "Changing blank state to" << blank;
  if ( blank ) {
    ui.imageLabel->clear();
  } else {
    emit rerenderRequested();
  }
}

bool PDFViewerWindow::isBlank() const
{
  return blank;
}

void PDFViewerWindow::setMyPagePart(const PagePart& newPagePart)
{
  this->myPart = newPagePart;
}

void PDFViewerWindow::parseLinks(QList< AdjustedLink > links)
{
  QList< HyperlinkArea* > newLinkAreas;
  for( AdjustedLink const & link: links ) {
    const QRectF& rect = link.linkArea();
    if ( rect.isNull() ) {
      qWarning() << "Null Link Area not supported yet.";
      continue;
    }
    const Poppler::Link::LinkType& type = link.link()->linkType();
    if ( type == Poppler::Link::LinkType::Goto ) {
      // type is Goto. Bind it to imageLabel
      const Poppler::LinkGoto& linkGoto = dynamic_cast<const Poppler::LinkGoto&>( * link.link() );
      if( linkGoto.isExternal() ) {
	qWarning() << "External links are not supported yet.";
	continue;
      }
      HyperlinkArea* linkArea = new HyperlinkArea(ui.imageLabel, link);
      sconnect( linkArea, SIGNAL(gotoPageRequested(uint)), this, SLOT(linkClicked(uint)) );
      newLinkAreas.append(linkArea);
    }
    else {
      qWarning() << "Types other than Goto are not supported yet.";
      continue;
    }
  }
  // Schedule all old links for deletion
  for( HyperlinkArea* hla: this->linkAreas)
    hla->deleteLater();
  // Add the new list
  this->linkAreas = newLinkAreas;
}

void PDFViewerWindow::linkClicked(uint targetNumber)
{
  DEBUGOUT << "Hyperlink detected";
  emit pageRequested(targetNumber);
}
