#include "testhelpers.hh"

using namespace std;
using namespace TestHelpers;

int main() {
	PDFDocumentReference pdr( TestHelpers::pdfFilename(), PDFCacheOption::keepPDFinMemory );

	auto left = RenderUtils::renderPagePart(pdr.page(0).page, QSize(1920,1080), PagePart::LeftHalf);
	auto right = RenderUtils::renderPagePart(pdr.page(0).page, QSize(1920,1080), PagePart::RightHalf);
	auto both = RenderUtils::renderPagePart(pdr.page(0).page, QSize(3840,1080), PagePart::FullPage);

	return expect(QSize(1920,1080), left.size())
		&& expect(QSize(1920,1080), right.size())
		&& expect(QSize(3840,1080), both.size());
}
