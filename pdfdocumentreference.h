#ifndef PDFDOCUMENTREFERENCE_H
#define PDFDOCUMENTREFERENCE_H
#include <QString>
#include <QSharedPointer>
#include "pdfcacheoption.h"
#include <poppler/qt4/poppler-qt4.h>

// forward-declare
struct PDFPageReference;

/** Holds a reference to a PDF document.
 * 
 * What exactly this means depends on the cache option:
 * If the cache option is hold in memory, this class wild have a
 * complete in-memory copy of the PDF file.
 * 
 * If the cache option is reread from disk, the reference is
 * just the file name.
 * 
 */
class PDFDocumentReference
{
private:
  const QString filename_;
  QByteArray fileContents_;
  const PDFCacheOption cacheOption_;
  
public:
  /** Create the document reference.
   * 
   * If the cache option is keep in memory, this will read the entire file into memory.
   */
  PDFDocumentReference( const QString& filename, const PDFCacheOption& cacheOption);
  
  /** Returns a Poppler::Page reference to the requested page number
   * NOTE: Until poppler supports multi-threading, this will
   * create a new Poppler::Document for each request.
   */
  PDFPageReference page(unsigned pageNumber) const;
  
  /** Create a Poppler::Document from this reference.
   * If you did not cache the file to memory, this step will try to
   * read the file.
   */
  QSharedPointer<Poppler::Document> popplerDocument() const;
  
  /** get filename */
  const QString& filename() const;
  
  /** get cache setting */
  const PDFCacheOption& cacheOption() const;
  
  /** Update the document reference.
   * This only works if rhs has the same filename and cache options,
   * effectively only updating the file contents buffer.
   */
  PDFDocumentReference& operator = (const PDFDocumentReference& rhs);
};

#endif // PDFDOCUMENTREFERENCE_H
