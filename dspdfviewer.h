#ifndef dspdfviewer_H
#define dspdfviewer_H

#include <QObject>
#include <poppler/qt4/poppler-qt4.h>

#include "pdfviewerwindow.h"
#include "pdfrenderfactory.h"

class DSPDFViewer: public QObject
{
  
private:
  QSharedPointer< Poppler::Document > pdfDocument;
  PDFViewerWindow audienceWindow;
  PDFViewerWindow secondaryWindow;
  
  
private:
  unsigned int m_pagenumber;
  PdfRenderFactory renderFactory;
  
public:
  static const QSize thumbnailSize;
  
private:
  QImage renderForTarget( QSharedPointer<Poppler::Page> page, QSize targetSize, bool onlyHalf, bool rightHalf=false);
  
Q_OBJECT
public:
    DSPDFViewer(QString filename, bool splitMode=true);
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
    
    PdfRenderFactory* theFactory();
};

#endif // dspdfviewer_H
