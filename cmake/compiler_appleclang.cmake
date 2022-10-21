message(WARNING "Optimal settings for AppleClang unknown, send pull-requests!")
message(STATUS "Including definitions for clang.")
include(cmake/compiler_clang.cmake)

# error: include location '/usr/local/include' is unsafe for cross-compilation
add_definitions(-Wno-error=poison-system-directories)
