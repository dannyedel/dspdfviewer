# MSVC

option(CompileMTMode "Build in MT mode" ON)
if(CompileMTMode)
	foreach(var CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
		CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
		if(${var} MATCHES "/MT")
			string(REGEX REPLACE "/MT" "/MD" ${var} "${${var}}")
		endif()
	endforeach()
endif()
