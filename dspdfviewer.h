#ifndef dspdfviewer_H
#define dspdfviewer_H

#include <QObject>
#include <poppler/qt4/poppler-qt4.h>

#include "pdfviewerwindow.h"
#include "pdfrenderfactory.h"
#include "runtimeconfiguration.h"

class DSPDFViewer: public QObject
{
private:
  const RuntimeConfiguration runtimeConfiguration;

private:
  QTimer	clockDisplayTimer;
  QTime 	slideStart;
  QTime		presentationStart;
  bool		presentationClocksRunning;

private:
  bool readyToRender;
  
  QSharedPointer< Poppler::Document > pdfDocument;
  PdfRenderFactory renderFactory;
  unsigned int m_pagenumber;
  PDFViewerWindow audienceWindow;
  PDFViewerWindow secondaryWindow;
  


public:
  static const QSize thumbnailSize;
  
private:
  QImage renderForTarget( QSharedPointer<Poppler::Page> page, QSize targetSize, bool onlyHalf, bool rightHalf=false);
  
  QString timeToString(QTime time);
  QString timeToString(int milliseconds);
  void	resetSlideClock();
  
Q_OBJECT
public:
    DSPDFViewer(const RuntimeConfiguration& r);
    virtual ~DSPDFViewer();
    
    /** (re-)Renders the current page on both monitors
     */
    void renderPage();
    
    /** goes to the specified page. Pages start at zero.
     */
    void gotoPage(unsigned int pageNumber);
    
    /** get current page number
     */
    unsigned int pageNumber();
    
    /** get page count
     */
    unsigned int numberOfPages();
    
    void setHighQuality(bool hq);
    
    /** go Forward one page
     */
    void goForward();
    
    /** go backward one page
     */
    void goBackward();
    
    void swapScreens();
    
    void exit();
    
    bool isReadyToRender();
    
    void goToStartAndResetClocks();
    
    PdfRenderFactory* theFactory();
    
    QString wallClock();
    QString slideClock();
    QString presentationClock();
};

#endif // dspdfviewer_H
