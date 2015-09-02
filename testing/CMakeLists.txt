# Add static library containing test helper functions
add_library(testhelp
	testhelpers
)

# Build the PDF before creating the test helper functions
add_custom_command(TARGET testhelp PRE_BUILD
	DEPENDS pdfs/colored-rectangles.tex
	COMMAND pdflatex -interaction=nonstopmode ${CMAKE_CURRENT_SOURCE_DIR}/pdfs/colored-rectangles.tex >/dev/null
	WORKING_DIRECTORY
)

# Link this helper library to dspdfviewer's functions
target_link_libraries(testhelp libdspdfviewer)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../ )


# Now add actual tests.
add_executable(testrenderonepage testrenderonepage.cc)
target_link_libraries(testrenderonepage testhelp)
add_test(RenderOnePage testrenderonepage)


add_executable(testswapscreen testswapscreen.cc)
target_link_libraries(testswapscreen testhelp)
#add_test(SwapScreenWithS testswapscreen)
