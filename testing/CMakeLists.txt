# Test driver library for convenience linking.
add_library(testhelp
	testhelpers
)
# Link this helper library to dspdfviewer's functions
target_link_libraries(testhelp
	libdspdfviewer
	${LIST_TEST_LIBRARIES}
)

list(APPEND PDFFILENAMES
	colored-rectangles.pdf
	images.pdf
	many-many-pages.pdf
)

if(DownloadTestPDF)
	file(DOWNLOAD http://danny-edel.de/colored-rectangles.pdf ${CMAKE_CURRENT_BINARY_DIR}/colored-rectangles.pdf
		TIMEOUT 30
		SHOW_PROGRESS
		EXPECTED_MD5 de9617fb1c00d6185e90314124e2c2f5
		)
	file(DOWNLOAD http://danny-edel.de/images.pdf ${CMAKE_CURRENT_BINARY_DIR}/images.pdf
		TIMEOUT 30
		SHOW_PROGESS
		EXPECTED_MD5 5c72e0954d457e3e9a1287ff299e307a
		)
	file(DOWNLOAD http://danny-edel.de/many-many-pages.pdf ${CMAKE_CURRENT_BINARY_DIR}/many-many-pages.pdf
		TIMEOUT 30
		SHOW_PROGRESS
		EXPECTED_MD5 22d934fbc13a93adc5e91bfec866229e
		)
else()
	# Compile from source
	find_program(PDFLATEX
		NAMES pdflatex
		)
	if( PDFLATEX STREQUAL "PDFLATEX-NOTFOUND")
		message(FATAL_ERROR "Compiling PDFs from source requires pdflatex with latex-beamer installed")
	endif()
	foreach(pdffile IN LISTS PDFFILENAMES)
		# get source file name.
		# replace .pdf with .tex ending,
		# prefix source directory
		# and replace any ~ characters in filename with \\string~
		STRING(REGEX REPLACE "\\.pdf$" ".tex" SOURCEFILENAME ${pdffile})
		SET(SOURCEDIRNAME "${CMAKE_CURRENT_SOURCE_DIR}/pdfs")
		SET(SOURCEFULLPATH "${SOURCEDIRNAME}/${SOURCEFILENAME}")
		STRING(REPLACE "~" "\\\\string~" PDFLATEX_FIXED_DIRNAME ${SOURCEDIRNAME})
		SET(PDFLATEX_FIXED_FILENAME "${PDFLATEX_FIXED_DIRNAME}/${SOURCEFILENAME}")

		add_custom_command(OUTPUT ${pdffile}
			DEPENDS ${SOURCEFULLPATH}
			COMMAND # env TEXINPUTS=${SOURCEDIRNAME}
				${PDFLATEX}
				-interaction=nonstopmode
				-output-directory ${CMAKE_CURRENT_BINARY_DIR}
				${PDFLATEX_FIXED_FILENAME}
				WORKING_DIRECTORY ${SOURCEDIRNAME}
		)

		message(STATUS "Building ${pdffile} from source.")
	endforeach()
endif()

foreach(pdffile IN LISTS PDFFILENAMES)
	list(APPEND PDFTARGETFILES ${CMAKE_CURRENT_BINARY_DIR}/${pdffile})
endforeach()
add_custom_target(testpdfs DEPENDS
	${PDFTARGETFILES}
)


add_dependencies(testhelp testpdfs)

# Allow the programs headers to be found using #include <...>
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../ )

# Now add actual tests.
add_executable(testrunner
	testrenderonepage.cc
	test-images.cc
	testrunner.cc
	test-thumbnail-cmdline.cc
)

target_link_libraries(testrunner testhelp)

# Clang/G++: Don't promote warnings to errors when building the test suite.
if(CMAKE_COMPILER_IS_GNUCXX)
	set_target_properties(testrunner
		PROPERTIES
			COMPILE_FLAGS "-Wno-error=effc++"
			)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" )
	set_target_properties(testrunner
		PROPERTIES
			COMPILE_FLAGS "-Wno-error=global-constructors"
			)
endif()

add_test(NAME BoostTestRunner
	COMMAND testrunner --report_level=detailed
)

# Check whether --version and --help contain
# the version in their output
add_test(NAME TestCommandLineVersion
	COMMAND dspdfviewer --version
)
add_test(NAME TestCommandLineHelp
	COMMAND dspdfviewer --help
)
set_tests_properties(
	TestCommandLineVersion
	TestCommandLineHelp
	PROPERTIES
		PASS_REGULAR_EXPRESSION ${DSPDFVIEWER_VERSION}
)

# Only build and run the swap screen test on Qt5
if(UseQtFive)
	add_executable(testswapscreen
		testswapscreen.cc
		testswapscreen-main.cc
	)
	target_link_libraries(testswapscreen
		testhelp
	)

	if(RunDualScreenTests)
		add_test(NAME testswapscreen
			COMMAND testswapscreen
				--prerender-prev=0
				--prerender-next=0
				images.pdf
		)
		set_tests_properties(testswapscreen
			PROPERTIES TIMEOUT 60)
	endif()
endif()


# On windows you cannot change the locale on a case-by-case basis.
#
# FIXME: Windows: Discover current locale
# FIXME: Windows: Include the translation test for the current locale

if(NOT WINDOWS)
	add_test(NAME EnglishByDefault
		COMMAND dspdfviewer --help
	)

	set(ENVVARS
		"LC_ALL=C.UTF-8:C"
		"LANGUAGE=C"
	)

	set_tests_properties(EnglishByDefault PROPERTIES
		PASS_REGULAR_EXPRESSION "Interactive Controls"
		ENVIRONMENT "${ENVVARS}"
		)

	# the ENVIRONMENT property was added in 2.8.0
	add_test(NAME GermanWithDeDE
		COMMAND dspdfviewer --help
	)

	set(ENVVARS
		"LC_ALL=de_DE.UTF-8:de_DE"
		"LANGUAGE=de"
	)

	set_tests_properties(GermanWithDeDE PROPERTIES
		PASS_REGULAR_EXPRESSION "Interaktive Tasten"
		ENVIRONMENT "${ENVVARS}"
	)
endif()
