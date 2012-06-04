#ifndef dspdfviewer_H
#define dspdfviewer_H

#include <QObject>
#include <poppler/qt4/poppler-qt4.h>

#include <memory> // shared pointer
#include "pdfviewerwindow.h"

class DSPDFViewer: public QObject
{
  
private:
  PDFViewerWindow primaryWindow;
  PDFViewerWindow secondaryWindow;
  
private:
  std::shared_ptr<Poppler::Document> pdfDocument;
  std::shared_ptr<Poppler::Page> currentPage;
  unsigned int m_pagenumber;
  
Q_OBJECT
public:
    DSPDFViewer(QString filename);
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
    
    /** go Forward one page
     */
    void goForward();
    
    /** go backward one page
     */
    void goBackward();
    
    void swapScreens();
    
    void exit();
};

#endif // dspdfviewer_H
