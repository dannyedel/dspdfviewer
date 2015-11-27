#include "testhelpers.hh"

// Test that the commandline accepts "left", "right" and "both",
// but not another string.

BOOST_AUTO_TEST_CASE(thumbnail_cmdline) {
	const int argc=3;

	// correct arguments, left
	{
		const char * const argv[] = {
			"dspdfviewer", "--thumbnail-page-part", "left"
		};
		RuntimeConfiguration rc{ argc, argv };
		BOOST_CHECK_EQUAL( rc.thumbnailPagePart(), PagePart::LeftHalf );
	}
	// shorthand, two words
	{
		const char * const argv[] = {
			"dspdfviewer", "-T", "left"
		};
		RuntimeConfiguration rc{ argc, argv };
		BOOST_CHECK_EQUAL( rc.thumbnailPagePart(), PagePart::LeftHalf );
	}
	// shorthand, one word
	{
		const char * const argv[] = {
			"dspdfviewer", "-Tleft"
		};
		RuntimeConfiguration rc{ argc, argv };
		BOOST_CHECK_EQUAL( rc.thumbnailPagePart(), PagePart::LeftHalf );
	}

	// right
	{
		const char* const argv[] = {
			"dspdfviewer", "--thumbnail-page-part", "right"
		};
		RuntimeConfiguration rc{ argc, argv };
		BOOST_CHECK_EQUAL( rc.thumbnailPagePart(), PagePart::RightHalf );
	}
	{
		const char* const argv[] = {
			"dspdfviewer", "--thumbnail-page-part", "both"
		};
		RuntimeConfiguration rc{ argc, argv };
		BOOST_CHECK_EQUAL( rc.thumbnailPagePart(), PagePart::FullPage );
	}

	// wrong argument
	{
		const char* const argv[] = {
			"dspdfviewer", "--thumbnail-page-part", "something"
		};
		BOOST_CHECK_THROW( RuntimeConfiguration( argc, argv ) , std::logic_error );
	}

	// no argument
	{
		const char* const argv[] = {
			"dspdfviewer", "--thumbnail-page-part"
		};
		BOOST_CHECK_THROW( RuntimeConfiguration( 2, argv) , std::logic_error );
	}
}

