# cmake fragment

# Compiler support

if(CMAKE_COMPILER_IS_GNUCXX)
	message(STATUS "GNU G++ detected.")
	include(cmake/compiler_gnu_gcc.cmake)

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	message(STATUS "Clang++ detected.")
	include(cmake/compiler_clang.cmake)

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

# Disable debug info unless we're in debugmode.
if( "${CMAKE_BUILD_TYPE}" MATCHES "^Debug$" )
  # do nothing
else()
  add_definitions(-DQT_NO_DEBUG_OUTPUT)
endif()


# This helps with translation, since it disables QString(const char*)
add_definitions(-DQT_NO_CAST_FROM_ASCII)
