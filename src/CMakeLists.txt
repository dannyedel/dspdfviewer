# /*
#     dspdfviewer - Dual Screen PDF Viewer for LaTeX-Beamer
#     Copyright (C) 2012  Danny Edel <mail@danny-edel.de>
#
#     This program is free software; you can redistribute it and/or modify
#     it under the terms of the GNU General Public License as published by
#     the Free Software Foundation; either version 2 of the License, or
#     (at your option) any later version.
#
#     This program is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU General Public License for more details.
#
#     You should have received a copy of the GNU General Public License along
#     with this program; if not, write to the Free Software Foundation, Inc.,
#     51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
# */

project(dspdfviewer)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)
find_library(POPPLER_LIBRARY poppler-qt4)
set(CMAKE_EXPORT_COMPILE_COMMANDS "on")

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})
include_directories(${Boost_INCLUDE_DIR})

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

if( "${CMAKE_BUILD_TYPE}" MATCHES "^Debug$" )
  # do nothing
else()
  add_definitions(-DQT_NO_DEBUG_OUTPUT)
endif()

if( NOT "${DSPDFVIEWER_VERSION}" MATCHES "^$" )
  # Not-Empty version given on the command line
  message(STATUS "Using the version number ${DSPDFVIEWER_VERSION} specified on the command line.")
else()
  # We dont have a version number given by the build system.
  # Try to figure it out first by asking git (only works for in-tree source builds)
  execute_process(COMMAND git describe
	  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_DESCRIBE_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
  if( NOT "${GIT_DESCRIBE_VERSION}" MATCHES "^$" )
    message(STATUS "Using version number ${GIT_DESCRIBE_VERSION} as defined by git-describe.")
    set(DSPDFVIEWER_VERSION ${GIT_DESCRIBE_VERSION})
  else()
	# use the standard utility dpkg-parsechangelog to get the version number
	execute_process(COMMAND dpkg-parsechangelog
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		OUTPUT_VARIABLE DEBIANCHANGELOG OUTPUT_STRIP_TRAILING_WHITESPACE)
	# grep for the line with "Version: "
	string(REGEX MATCH "Version: [^ \r\n\t]*" DEBIANVERSION "${DEBIANCHANGELOG}")
	# strip "Version: "
	string(REPLACE "Version: " "" DEBIANVERSION "${DEBIANVERSION}")
    if ( NOT "${DEBIANVERSION}" MATCHES "^$" )
      message(STATUS "Using version number ${DEBIANVERSION} as parsed from debian/changelog's first entry.")
      set(DSPDFVIEWER_VERSION ${DEBIANVERSION})
    else()
      # Try to infer the version number from the debian/changelog source file
      message(WARNING "Could not determine the version number of the program. Please pass -DDSPDFVIEWER_VERSION=1.2.3.4.5 to the cmake command.")
    endif()
  endif()
endif()

if( NOT "${DSPDFVIEWER_VERSION}" MATCHES "^$" )
  add_definitions(-DDSPDFVIEWER_VERSION="${DSPDFVIEWER_VERSION}")
endif()


set(dspdfviewer_SRCS adjustedlink.cpp hyperlinkarea.cpp pdfpagereference.cpp pdfdocumentreference.cpp runtimeconfiguration.cpp renderutils.cpp renderthread.cpp renderingidentifier.cpp pagepart.cpp renderedpage.cpp pdfrenderfactory.cpp pdfviewerwindow.cpp dspdfviewer.cpp main.cpp)

qt4_wrap_ui(dspdfviewer_UIS_H pdfviewerwindow.ui)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
qt4_automoc(${dspdfviewer_SRCS} )
add_executable(dspdfviewer ${dspdfviewer_SRCS} ${dspdfviewer_UIS_H})
target_link_libraries(dspdfviewer ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${Boost_LIBRARIES} ${POPPLER_LIBRARY})

install(TARGETS	dspdfviewer
	RUNTIME DESTINATION bin)

install(FILES docs/dspdfviewer.1
	DESTINATION share/man/man1)
