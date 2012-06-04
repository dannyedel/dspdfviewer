#ifndef dspdfviewer_H
#define dspdfviewer_H

#include <QObject>
#include "primarywindow.h"
#include "secondarywindow.h"
#include <poppler/qt4/poppler-qt4.h>

#include <memory> // shared pointer

class DSPDFViewer: public QObject
{
  
private:
  PrimaryWindow primaryWindow;
  SecondaryWindow secondaryWindow;
  
private:
  std::shared_ptr<Poppler::Document> pdfDocument;
  std::shared_ptr<Poppler::Page> currentPage;
  
Q_OBJECT
public:
    DSPDFViewer(QString filename);
    virtual ~DSPDFViewer();
    
};

#endif // dspdfviewer_H
