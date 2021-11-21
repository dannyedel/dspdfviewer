# GNU GCC

include(CheckCXXCompilerFlag)

CHECK_CXX_COMPILER_FLAG("-std=c++11" C11FLAG)
if(C11FLAG)
	message(STATUS "Compiler supports -std=c++11, using it.")
	add_definitions(-std=c++11)
else()
	message(WARNING "Compiler does not support -std=c++11, trying c++0x")
	CHECK_CXX_COMPILER_FLAG("-std=c++0x" C0XFLAG)
	if(C0XFLAG)
		message(STATUS "Using -std=c++0x")
		add_definitions(-std=c++0x)
		# Abuse the preprocessor to remove the
		# override keyword on this old compiler.
		add_definitions(-Doverride= )
	else()
		message(FATAL_ERROR "Compiler does not support -std=c++0x either. "
			"Please upgrade your compiler."
			)
	endif()
endif()

# Set as much warnings as possible.
add_definitions(
	-Wall
	-Wextra
	-pedantic
	-Wold-style-cast
	-Woverloaded-virtual
	-Weffc++
)

# Turn all warnings into errors
add_definitions(-Werror -pedantic-errors)

# These warnings produce false positives on some older qt4 libraries
# (this failed debian s390x compilation), therefore tune them back
# to warning when building against qt4.

if( NOT UseQtFive )
	add_definitions(-Wno-error=old-style-cast)
	add_definitions(-Wno-error=effc++)
endif()

if(CodeCoverage)
	message(STATUS "Adding gcov as test coverage helper")
	add_definitions(-fprofile-arcs -ftest-coverage -O0)
	LIST(APPEND LIST_LIBRARIES
		gcov
	)
endif()
