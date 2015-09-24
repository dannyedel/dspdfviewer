# GNU GCC

# Found on
# http://stackoverflow.com/questions/10984442/how-to-detect-c11-support-of-a-compiler-with-cmake
#
# Identify GCC version
execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)

# GCC >= 4.7 supports -std=c++11 argument
if (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7)
	message(STATUS "GNU GCC 4.7 or higher detected.")
	add_definitions("-std=c++11")

# On GCC >= 4.3 it is supported as -std=c++0x, but some things aren't supported
# yet (this will fail later on when compiling)
elseif(GCC_VERSION VERSION_GREATER 4.3 OR GCC_VERSION VERSION_EQUAL 4.3)
	message(WARNING "You are using an old version of gcc."
		" Consider upgrading to a more recent version if you run into problems.")
	add_definitions("-std=c++0x")

# Even older version: Fail now.
else ()
	message(FATAL_ERROR "C++11 needed. For GCC that means a version higher than 4.3 is needed.")
endif()


# Set as much warnings as possible.
add_definitions(
	-Wall
	-Wextra
	-Wconversion
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

# Add the include directories using -isystem
# This should™ supress warnings in included files

foreach(lib IN LISTS LIST_INCLUDE_DIRS)
	add_definitions(-isystem ${lib})
endforeach()


if(CodeCoverage)
	message(STATUS "Adding gcov as test coverage helper")
	add_definitions(-fprofile-arcs -ftest-coverage -O0)
	LIST(APPEND LIST_LIBRARIES
		gcov
	)
endif()
