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


#ifndef PDFVIEWERWINDOW_H
#define PDFVIEWERWINDOW_H

#include "renderedpage.h"
#include "pdfrenderfactory.h"
#include "runtimeconfiguration.h"
#include "hyperlinkarea.h"
#include "windowrole.h"

#include "ui_pdfviewerwindow.h"

/** Shared base class for both windows (primary and secondary)
 *
 */
class PDFViewerWindow : public QWidget
{
  Q_OBJECT
private:
  Ui::Form ui;
  bool	m_enabled;
  QImage currentImage;
  bool blank;
  bool informationLineVisible;

  uint currentPageNumber;
  uint minimumPageNumber;
  uint maximumPageNumber;
  bool correctImageRendered;
  PagePart myPart;

  const WindowRole windowRole;
  // Reference to the runtime configuration object.
  const RuntimeConfiguration& runtimeConfiguration;

  /** Display this image
    */
  void displayImage(QImage image);

  virtual void wheelEvent(QWheelEvent* e);

  virtual void keyPressEvent(QKeyEvent* e);

  virtual void mousePressEvent(QMouseEvent* e);

  void addThumbnail(uint pageNumber, QImage thumbnail);


  QString timeToString(const QTime& time) const;
  QString timeToString(int milliseconds) const;

  void keybindingsPopup();
  void changePageNumberDialog();

  void parseLinks( QList< AdjustedLink > links);
  QList< HyperlinkArea* > linkAreas;
  
public:
  /** Standard constructor
   */
    explicit PDFViewerWindow(PagePart myPart, bool showInformationLine, const RuntimeConfiguration& r, const WindowRole& windowRole, bool enabled=true);

    QSize getTargetImageSize() const;

    QSize getPreviewImageSize();
    
    PagePart getMyPagePart() const;

    void showInformationLine();

    void hideInformationLine();

    bool isInformationLineVisible() const;

    bool isBlank() const;

    void showLoadingScreen(uint pageNumberToWaitFor);

public slots:
  void renderedPageIncoming( QSharedPointer<RenderedPage> renderedPage);

  void resizeEvent(QResizeEvent* resizeEvent);

  void updateWallClock(const QTime& wallClock);
  void updateSlideClock(const QTime& slideClock);
  void updatePresentationClock(const QTime& presentationClock);

  void setPageNumberLimits(uint minimumPageNumber, uint maximumPageNumber);

  void setBlank(const bool blank);

  void setMyPagePart(const PagePart& newPagePart);

  signals:
    void nextPageRequested();
    void previousPageRequested();
    void pageRequested(unsigned requestedPageNumber);
    void restartRequested();

    void screenSwapRequested();

    void rerenderRequested();

    void quitRequested();
    void secondScreenFunctionToggleRequested();
    void secondScreenDuplicateRequested();

    void blankToggleRequested();
private slots:
  void linkClicked(uint targetNumber);
};

#endif // PDFVIEWERWINDOW_H
