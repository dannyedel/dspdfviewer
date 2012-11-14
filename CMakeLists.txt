project(dspdfviewer)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

add_definitions(-ggdb)

# Use c++11 support.
# found on 
# http://stackoverflow.com/questions/10984442/how-to-detect-c11-support-of-a-compiler-with-cmake
if(CMAKE_COMPILER_IS_GNUCXX)
	execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
	if (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7)
		message(STATUS "C++11 activated.")
		add_definitions("-std=c++11")
	elseif(GCC_VERSION VERSION_GREATER 4.3 OR GCC_VERSION VERSION_EQUAL 4.3)
		message(WARNING "C++0x activated. If you get any errors update to a compiler which fully supports C++11")
		add_definitions("-std=c++0x")
	else ()
		message(FATAL_ERROR "C++11 needed. Therefore a gcc compiler with a version higher than 4.3 is needed.")   
	endif()
else()
	# No GCC Compiler
	message(WARNING "You dont have a GNU compiler. I'll activate -std=c++11 in the hope it does exactly that.")
	add_definitions(-std=c++11)
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
	#add some warnings
	add_definitions(-Wall -Wextra -pedantic -Werror -ggdb -Wfatal-errors -Wold-style-cast -Woverloaded-virtual)
	#I like these for my code, but Qt doesnt compile with them
	# add_definitions( -Weffc++ )
	add_definitions(-Wno-effc++)
else()
	message(WARNING "Compiling with a Non-GNU compiler. A lot less warnings will be output, so more coding errors might go undetected.")
endif()

set(dspdfviewer_SRCS renderutils.cpp renderthread.cpp renderingidentifier.cpp pagepart.cpp renderedpage.cpp pdfrenderfactory.cpp pdfviewerwindow.cpp dspdfviewer.cpp main.cpp)
qt4_automoc(${dspdfviewer_SRCS})
add_executable(dspdfviewer ${dspdfviewer_SRCS})
target_link_libraries(dspdfviewer ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} poppler-qt4)

install(TARGETS	dspdfviewer
	RUNTIME DESTINATION bin)
