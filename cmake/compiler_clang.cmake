# CMake fragment

# Clang
add_definitions("-std=c++11")

# Turn on a lot of warnings, hopefully helping with code quality.
add_definitions(-Weverything)
# And turn them into errors.
add_definitions(-Werror)

# Disable warnings irrelevant to the project:
#   c++98 compatibility warnings
add_definitions(-Wno-c++98-compat -Wno-c++98-compat-pedantic)

# Currently this warning triggers in exception classes
add_definitions(-Wno-error=weak-vtables)

# We don't care about padding right now.
add_definitions(-Wno-padded)

# in release mode: Unrechable code / Macro expansion (these stem from QDEBUG)
# trigger a lot (QDEBUG compiles into the equivalent of while(false) { ... } )
# So these are safe to ignore IFF we're not in Debug mode.
if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug" )
	add_definitions(-Wno-unreachable-code -Wno-disabled-macro-expansion)
endif()

# Qt's moc (Meta Object Compiler) generates code that triggers warnings
# about undefined behaviours.

# So don't set Werror for it.
# TODO: Set this only for the automoc files
add_definitions(-Wno-error=undefined-reinterpret-cast)

# qrc system generates code that triggers a lot of the
# clang warnings.
set_source_files_properties( ${EMBEDDED_QRC}
	PROPERTIES COMPILE_FLAGS "-Wno-error")
