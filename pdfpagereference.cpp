#include "pdfpagereference.h"

#include <boost/numeric/conversion/cast.hpp>

using boost::numeric_cast;

PDFPageReference::PDFPageReference(const PDFDocumentReference& documentReference, const unsigned int& pageNumber):
  document( documentReference.popplerDocument() ),
  page( document->page( numeric_cast<int>(pageNumber) ) )
{
}
