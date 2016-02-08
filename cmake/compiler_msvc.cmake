# MSVC

foreach(var
	CMAKE_C_FLAGS
	CMAKE_C_FLAGS_DEBUG
	CMAKE_C_FLAGS_RELEASE
	CMAKE_C_FLAGS_MINSIZEREL
	CMAKE_C_FLAGS_RELWITHDEBINFO
	CMAKE_CXX_FLAGS
	CMAKE_CXX_FLAGS_DEBUG
	CMAKE_CXX_FLAGS_RELEASE
	CMAKE_CXX_FLAGS_MINSIZEREL
	CMAKE_CXX_FLAGS_RELWITHDEBINFO
)
	if(WindowsStaticLink)
		if(${var} MATCHES "/MD")
			string(REGEX REPLACE "/MD" "/MT" ${var} "${${var}}")
		endif()
	else() # not WindowsStaticLink
		if(${var} MATCHES "/MT")
			string(REGEX REPLACE "/MT" "/MD" ${var} "${${var}}")
		endif()
	endif()
endforeach()
