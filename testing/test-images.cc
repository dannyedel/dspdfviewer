#include "testhelpers.hh"

using namespace std;
using namespace TestHelpers;

BOOST_AUTO_TEST_CASE(image_rendering) {
	PDFDocumentReference pdr( TestHelpers::pdfFilename("images.pdf"), PDFCacheOption::keepPDFinMemory );

	auto first = RenderUtils::renderPagePart(pdr.page(0).page, QSize(1920,1080), PagePart::LeftHalf);
	auto second = RenderUtils::renderPagePart(pdr.page(1).page, QSize(1920,1080), PagePart::LeftHalf);

	auto firstColor = QColor( 0x70, 0x1e, 0xc1).rgb();
	auto secondColor = QColor( 0x14, 0x63, 0xb4).rgb();
	
	/** Check sizes of rendered images **/
	BOOST_CHECK_EQUAL( QSize(1920,1080), first.size());
	BOOST_CHECK_EQUAL( QSize(1920,1080), second.size());

	/** Check pixels in the middle */
	BOOST_CHECK_EQUAL( firstColor, first.pixel(960,540));
	BOOST_CHECK_EQUAL( secondColor, second.pixel(960,540));
}
