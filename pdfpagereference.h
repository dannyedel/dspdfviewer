#ifndef PDFPAGEREFERENCE_H
#define PDFPAGEREFERENCE_H

#include <QSharedPointer>
#include "poppler-qt.h"
#include "pdfdocumentreference.h"

/** This holds a reference to a page in a certain document
 * WARNING: This also contains a shared pointer to the Poppler::Document,
 * so if you delete this object, using the returned page() is undefined
 * behaviour.
 */
struct PDFPageReference
{
  const QSharedPointer<const Poppler::Document> document;
  const QSharedPointer<const Poppler::Page> page;


  PDFPageReference( const PDFDocumentReference& documentReference, const unsigned int& pageNumber );
};

#endif // PDFPAGEREFERENCE_H
