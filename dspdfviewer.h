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


#ifndef dspdfviewer_H
#define dspdfviewer_H

#include <QObject>
#include <QTimer>
#include <QFileSystemWatcher>
#include "poppler-qt.h"

#include "pdfviewerwindow.h"
#include "pdfrenderfactory.h"
#include "runtimeconfiguration.h"

class DSPDFViewer: public QObject
{
  Q_OBJECT

private:
  const RuntimeConfiguration runtimeConfiguration;
  enum {
    TIMER_UPDATE_INTERVAL=250
  };

private:
  QTimer	clockDisplayTimer;
  QTime 	slideStart;
  QTime		presentationStart;
  bool		presentationClockRunning;

private:
  QFileSystemWatcher documentFileWatcher;
  PdfRenderFactory renderFactory;
  unsigned int m_pagenumber;
  PDFViewerWindow audienceWindow;
  PDFViewerWindow secondaryWindow;



public:
  static const QSize thumbnailSize;

private:
  QImage renderForTarget( QSharedPointer<Poppler::Page> page, QSize targetSize, bool onlyHalf, bool rightHalf=false);

  QString timeToString(QTime time) const;
  QString timeToString(int milliseconds) const;
  void	resetSlideClock();

  RenderingIdentifier toRenderIdent(unsigned int pageNumber, const PDFViewerWindow& window);
  RenderingIdentifier toThumbnailRenderIdent(unsigned int pageNumber, PDFViewerWindow& window);

private slots:
  void sendAllClockSignals() const;

public:
    DSPDFViewer(const RuntimeConfiguration& r);
    virtual ~DSPDFViewer();


    /** get current page number
     */
    unsigned int pageNumber() const;

    /** get page count
     */
    unsigned int numberOfPages() const;

    void setHighQuality(bool hq);

    bool isReadyToRender() const;

    PdfRenderFactory* theFactory();

    QTime wallClock() const;
    QTime slideClock() const;
    QTime presentationClock() const;

    QTime timeSince( const QTime& startPoint) const;

    bool isAudienceScreenBlank() const;

signals:
  void wallClockUpdate(const QTime& wallClock) const;
  void slideClockUpdate(const QTime& slideClock) const;
  void presentationClockUpdate(const QTime& presentationClock) const;

public slots:
    /** (re-)Renders the current page on both monitors
     */
    void renderPage();

    /** goes to the specified page. Pages start at zero.
     */
    void gotoPage(unsigned int pageNumber);
    /** go Forward one page
     */
    void goForward();

    /** go backward one page
     */
    void goBackward();

    void goToStartAndResetClocks();

    void swapScreens();

    void toggleAudienceScreenBlank();
    void setAudienceScreenBlank();
    void setAudienceScreenVisible();

    void toggleSecondaryScreenFunction();

    void exit();
};

#endif // dspdfviewer_H
