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
	add_definitions(-Wall -Wextra -pedantic -Werror -Wfatal-errors -Wold-style-cast -Woverloaded-virtual)
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

### Version-number inclusion logic.

# Rationale: dspdfviewer --version should print something meaningful
# Especially on builds from git, it should include the git revision.

if( DSPDFVIEWER_VERSION )
  # Not-Empty version given on the command line. This has absolute priority.
  message(STATUS "Embedding the version number ${DSPDFVIEWER_VERSION} specified on the command line.")
endif()

if( NOT DSPDFVIEWER_VERSION AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git" )
	# We don't have a version number yet, but
	# this looks like a checkout from git.
	# Ask "git describe" for a version number.
	message(STATUS "Building from a git clone, using git describe for a version number.")
	execute_process(COMMAND git describe
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		OUTPUT_VARIABLE GIT_DESCRIBE_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
	# Check if it gave something back
	if( NOT "${GIT_DESCRIBE_VERSION}" MATCHES "^$" )
		message(STATUS "Embedding version number ${GIT_DESCRIBE_VERSION} as defined by git-describe.")
		set(DSPDFVIEWER_VERSION ${GIT_DESCRIBE_VERSION})
	endif()
endif()

if( NOT DSPDFVIEWER_VERSION )
	# We still don't know version number to embed.
	# Use default

	# TODO: Keep me updated!
	set(DSPDFVIEWER_VERSION "1.14-unreleased")
	message(STATUS "Embedding version number ${DSPDFVIEWER_VERSION}. If you want to override this, "
		"for example to embed the git revision you built from, please pass "
		"-DDSPDFVIEWER_VERSION=1.2.3.4.5 to the cmake command.")
endif()

if( NOT "${DSPDFVIEWER_VERSION}" MATCHES "^$" )
  add_definitions(-DDSPDFVIEWER_VERSION="${DSPDFVIEWER_VERSION}")
endif()

if( I3WORKAROUND_SHELLCODE )
	add_definitions(-DI3WORKAROUND_SHELLCODE="${I3WORKAROUND_SHELLCODE}")
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

install(FILES dspdfviewer.desktop
	DESTINATION share/applications)
