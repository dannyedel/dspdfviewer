# If building on MSVC, WIN32 is defined, but on Cygwin/MSYS/MinGW it is not.

# use the WINDOWS variable for everything on windows (regardless of which
# compiler type), MSVC or MINGW for compiler-specific things.

message(STATUS "Compiling for system ${CMAKE_SYSTEM_NAME}")
if( CMAKE_SYSTEM_NAME STREQUAL "Windows" )
	set(WINDOWS ON)
elseif( CMAKE_SYSTEM_NAME STREQUAL "MSYS" )
	set(WINDOWS ON)
else()
	set(WINDOWS OFF)
endif()
