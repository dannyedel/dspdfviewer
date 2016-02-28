# cmake fragment

# Compiler support

if(CMAKE_COMPILER_IS_GNUCXX)
	message(STATUS "GNU G++ detected.")
	include(cmake/compiler_gnu_gcc.cmake)

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	message(STATUS "Clang++ detected.")
	include(cmake/compiler_clang.cmake)

elseif(MSVC)
	message(STATUS "MSVC detected.")
	include(cmake/compiler_msvc.cmake)

else()
	# Unknown Compiler
	message(WARNING "You are using an unidentified and therefore unsupported compiler. "
		" If this works for you, please report that to upstream and - if possible -"
		" please send patches activating sensible compile flags on it.")
	include(cmake/compiler_unknown.cmake)
endif()

#
### Common for all compilers
#

# Add -DQT_NO_DEBUG_OUTPUT to Release
# and Release with Debug Info builds

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DQT_NO_DEBUG_OUTPUT")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -DQT_NO_DEBUG_OUTPUT")

# This helps with translation, since it disables QString(const char*)
add_definitions(-DQT_NO_CAST_FROM_ASCII)
