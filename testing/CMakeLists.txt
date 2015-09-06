# Test driver library for convenience linking.
add_library(testhelp
	testhelpers
)
# Link this helper library to dspdfviewer's functions
target_link_libraries(testhelp libdspdfviewer)


# Build the PDF before creating the test helper functions
add_custom_command(OUTPUT colored-rectangles.pdf
	DEPENDS pdfs/colored-rectangles.tex
	COMMAND pdflatex -interaction=nonstopmode ${CMAKE_CURRENT_SOURCE_DIR}/pdfs/colored-rectangles.tex >/dev/null
	WORKING_DIRECTORY
)
add_custom_target(testpdfs DEPENDS colored-rectangles.pdf)
add_dependencies(testhelp testpdfs)

# Allow the programs headers to be found using #include <...>
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../ )

# Now add actual tests.
add_executable(testrunner
	testrenderonepage.cc
)

target_link_libraries(testrunner testhelp)

add_test(NAME BoostTestRunner
	COMMAND testrunner --report_level=detailed
)
