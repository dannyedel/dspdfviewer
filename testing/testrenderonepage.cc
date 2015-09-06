#include "testhelpers.hh"

using namespace std;
using namespace TestHelpers;

int main() {
	PDFDocumentReference pdr( TestHelpers::pdfFilename(), PDFCacheOption::keepPDFinMemory );

	auto left = RenderUtils::renderPagePart(pdr.page(0).page, QSize(1920,1080), PagePart::LeftHalf);
	auto right = RenderUtils::renderPagePart(pdr.page(0).page, QSize(1920,1200), PagePart::RightHalf);
	auto both = RenderUtils::renderPagePart(pdr.page(0).page, QSize(3840,1080), PagePart::FullPage);

	auto leftScreenColor = QColor( 0x88, 0xff, 0x88).rgb();
	auto rightScreenColor = QColor( 0xff, 0x88, 0xff).rgb();

	return
		/* Sizes for left and right should be exactly 16:9 full-hd */
		expect(QSize(1920,1080), left.size())
		&& expect(QSize(1920,1080), right.size())
		/* Size for both should be double-full-hd */
		&& expect(QSize(3840,1080), both.size())
		
		/* All-the-way-left and -right pixels should have the same color */
		&& expect(leftScreenColor, left.pixel(0, 500))
		&& expect(leftScreenColor, left.pixel(1919,500))

		&& expect(rightScreenColor, right.pixel(0,500))
		&& expect(rightScreenColor, right.pixel(1919,500))

		&& expect(leftScreenColor, both.pixel(0,500))
		&& expect(rightScreenColor, both.pixel(3839, 500))
		;
}
