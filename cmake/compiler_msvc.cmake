# MSVC

option(CompileMTMode "Build in MT mode" ON)
if(CompileMTMode)
	foreach(var CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
		CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
		if(${var} MATCHES "/MD")
			string(REGEX REPLACE "/MD" "/MT" ${var} "${${var}}")
		endif()
	endforeach()
endif()

include_directories(${LIST_INCLUDE_DIRS})
