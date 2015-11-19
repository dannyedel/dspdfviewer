# Test driver library for convenience linking.
add_library(testhelp
	testhelpers
)
# Link this helper library to dspdfviewer's functions
target_link_libraries(testhelp libdspdfviewer)

list(APPEND PDFFILENAMES
	colored-rectangles.pdf
	images.pdf
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
else()
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
				pdflatex
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

# Check that the translations get activated

if(APPLE)
	set(XVFB "")
elseif(WIN32)
	set(XFFB "")
else()
	set(XVFB "xvfb-run -a")
endif()
separate_arguments(XVFB)

add_test(NAME EnglishByDefault
	COMMAND env LANGUAGE=C LC_ALL=C.UTF-8 ${XVFB} ../dspdfviewer --help
)

set_tests_properties(EnglishByDefault PROPERTIES
	PASS_REGULAR_EXPRESSION "Interactive Controls"
	)

add_test(NAME GermanWithDeDE
	COMMAND env LANGUAGE=de LC_ALL=de_DE.UTF-8 ${XVFB} ../dspdfviewer --help
)

set_tests_properties(GermanWithDeDE PROPERTIES
	PASS_REGULAR_EXPRESSION "Interaktive Tasten"
)
