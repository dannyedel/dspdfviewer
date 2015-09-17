# Test driver library for convenience linking.
add_library(testhelp
	testhelpers
)
# Link this helper library to dspdfviewer's functions
target_link_libraries(testhelp libdspdfviewer)

option(DownloadTestPDF "Download test PDFs from http://danny-edel.de, rather then building them using latex-beamer" OFF)

if(DownloadTestPDF)
	file(DOWNLOAD http://danny-edel.de/colored-rectangles.pdf ${CMAKE_CURRENT_BINARY_DIR}/colored-rectangles.pdf
		TIMEOUT 30
		SHOW_PROGRESS
		EXPECTED_MD5 de9617fb1c00d6185e90314124e2c2f5
		)
	#	add_custom_command(OUTPUT colored-rectangles.pdf
	#	DEPENDS colored-rectangles.pdf.tmp
	#	COMMAND cp colored-rectangles.pdf.tmp colored-rectangles.pdf
	#	WORKING_DIRECTORY
	#	)
	add_custom_target(testpdfs DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/colored-rectangles.pdf)
else()
	# Build the PDF before creating the test helper functions
	add_custom_command(OUTPUT colored-rectangles.pdf
		DEPENDS pdfs/colored-rectangles.tex
		COMMAND pdflatex -interaction=nonstopmode ${CMAKE_CURRENT_SOURCE_DIR}/pdfs/colored-rectangles.tex >/dev/null
		WORKING_DIRECTORY
	)
	add_custom_target(testpdfs DEPENDS colored-rectangles.pdf)
endif()

add_dependencies(testhelp testpdfs)

# Allow the programs headers to be found using #include <...>
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../ )

# Now add actual tests.
add_executable(testrunner
	testrenderonepage.cc
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
