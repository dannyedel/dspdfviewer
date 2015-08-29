#include "testhelpers.hh"

int main() {
	PDFDocumentReference pdr( TestHelpers::pdfFilename(), PDFCacheOption::keepPDFinMemory );

	auto left = RenderUtils::renderPagePart(pdr.page(0).page, QSize(1920,1080), PagePart::LeftHalf);

	return 0;
}
