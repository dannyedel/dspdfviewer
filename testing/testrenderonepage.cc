#include "testhelpers.hh"

using namespace std;
using namespace TestHelpers;

BOOST_AUTO_TEST_CASE(render_one_page) {
	PDFDocumentReference pdr( TestHelpers::pdfFilename("colored-rectangles.pdf"), PDFCacheOption::keepPDFinMemory );

	const auto pageref = pdr.page(0);

	const auto left = RenderUtils::renderPagePart(pageref.page, QSize(1920,1080), PagePart::LeftHalf);
	const auto right = RenderUtils::renderPagePart(pageref.page, QSize(1920,1200), PagePart::RightHalf);
	const auto both = RenderUtils::renderPagePart(pageref.page, QSize(3840,1080), PagePart::FullPage);

	const auto leftScreenColor = QColor( 0x88, 0xff, 0x88);
	auto rightScreenColor = QColor( 0xff, 0x88, 0xff);

	/** Older texlive installation: Note page gets rendered white
	 */
	const auto fallbackRightScreenColor = QColor( 0xff, 0xff, 0xff);

	BOOST_CHECK_NE(rightScreenColor, fallbackRightScreenColor);

	/** Check sizes of rendered images **/
	BOOST_CHECK_EQUAL( QSize(1920,1080), left.size());
	BOOST_CHECK_EQUAL( QSize(1920,1080), right.size());
	BOOST_CHECK_EQUAL( QSize(3840,1080), both.size());

	/** Check middle pixel of left screen **/
	BOOST_CHECK_EQUAL( leftScreenColor, QColor(left.pixel(960,540)));
	BOOST_CHECK_EQUAL( leftScreenColor, QColor(both.pixel(960,540)));

	const auto rightScreenMiddle = QColor(right.pixel(960,540));

	BOOST_WARN_EQUAL( rightScreenColor, rightScreenMiddle );
	/** Check middle pixel of right screen **/
	if ( rightScreenMiddle == fallbackRightScreenColor) {
		BOOST_TEST_MESSAGE( "Middle of note screen was peak white, "
			", using workaround for older latex-beamer versions.");
		rightScreenColor = fallbackRightScreenColor;
	}
	BOOST_CHECK_EQUAL( rightScreenColor, QColor(right.pixel(960,540)));
	BOOST_CHECK_EQUAL( rightScreenColor, QColor(both.pixel(2880,540)));

	/** Check all-the-way-left and all-the-way-right pixel colors */
	BOOST_CHECK_EQUAL( leftScreenColor, QColor(left.pixel(0,540)));
	BOOST_CHECK_EQUAL( leftScreenColor, QColor(left.pixel(1919,540)));

	BOOST_CHECK_EQUAL( rightScreenColor, QColor(right.pixel(0,540)));
	BOOST_CHECK_EQUAL( rightScreenColor, QColor(right.pixel(1919,540)));

	BOOST_CHECK_EQUAL( leftScreenColor, QColor(both.pixel(0,540)));
	BOOST_CHECK_EQUAL( rightScreenColor, QColor(both.pixel(3839,540)));
}
