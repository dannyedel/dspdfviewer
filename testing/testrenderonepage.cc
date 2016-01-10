#include "testhelpers.hh"

using namespace std;
using namespace TestHelpers;

BOOST_AUTO_TEST_CASE(render_one_page) {
	PDFDocumentReference pdr( TestHelpers::pdfFilename("colored-rectangles.pdf"), PDFCacheOption::keepPDFinMemory );

	auto left = RenderUtils::renderPagePart(pdr.page(0).page, QSize(1920,1080), PagePart::LeftHalf);
	auto right = RenderUtils::renderPagePart(pdr.page(0).page, QSize(1920,1200), PagePart::RightHalf);
	auto both = RenderUtils::renderPagePart(pdr.page(0).page, QSize(3840,1080), PagePart::FullPage);

	auto leftScreenColor = QColor( 0x88, 0xff, 0x88);
	auto rightScreenColor = QColor( 0xff, 0x88, 0xff);

	/** Check sizes of rendered images **/
	BOOST_CHECK_EQUAL( QSize(1920,1080), left.size());
	BOOST_CHECK_EQUAL( QSize(1920,1080), right.size());
	BOOST_CHECK_EQUAL( QSize(3840,1080), both.size());

	/** Check pixels in the middle */
	BOOST_CHECK_EQUAL( leftScreenColor, QColor(left.pixel(960,540)));
	BOOST_CHECK_EQUAL( rightScreenColor, QColor(right.pixel(960,540)));
	BOOST_CHECK_EQUAL( leftScreenColor, QColor(both.pixel(960,540)));
	BOOST_CHECK_EQUAL( rightScreenColor, QColor(both.pixel(2880,540)));

	/** Check all-the-way-left and all-the-way-right pixel colors */
	BOOST_CHECK_EQUAL( leftScreenColor, QColor(left.pixel(0,540)));
	BOOST_CHECK_EQUAL( leftScreenColor, QColor(left.pixel(1919,540)));

	BOOST_CHECK_EQUAL( rightScreenColor, QColor(right.pixel(0,540)));
	BOOST_CHECK_EQUAL( rightScreenColor, QColor(right.pixel(1919,540)));

	BOOST_CHECK_EQUAL( leftScreenColor, QColor(both.pixel(0,540)));
	BOOST_CHECK_EQUAL( rightScreenColor, QColor(both.pixel(3839,540)));
}
