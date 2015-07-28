#include "pdfpagereference.h"

PDFPageReference::PDFPageReference(const PDFDocumentReference& documentReference, const unsigned int& pageNumber):
  document( documentReference.popplerDocument() ),
  page( document->page(pageNumber) )
{
}
