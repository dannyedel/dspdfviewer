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
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
include(FindPkgConfig)
set(CMAKE_AUTOMOC "on")#
set(CMAKE_EXPORT_COMPILE_COMMANDS "on")

option(UseQtFive "Build with Qt5 and libpoppler-qt5" OFF)

option(BuildTests "Build unit tests (this requires pdflatex)" ON)

if(BuildTests)
	find_package(Boost COMPONENTS program_options unit_test_framework REQUIRED)
else()
	find_package(Boost COMPONENTS program_options REQUIRED)
endif()

if(UseQtFive)
	#qt5
	message(STATUS "Using Qt5 and libpoppler-qt5")
	find_package(Qt5Core 5.1.1 REQUIRED)
	# On lower versions, QTBUG-32100 prevents compilation at least on clang
	# https://bugreports.qt.io/browse/QTBUG-32100
	find_package(Qt5Gui REQUIRED)
	find_package(Qt5Widgets REQUIRED)
	pkg_search_module(POPPLER REQUIRED poppler-qt5)
	# add their include directories
	list(APPEND LIST_INCLUDE_DIRS ${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS})
	# add their link flags
	list(APPEND LIST_LIBRARIES ${Qt5Core_LIBRARIES} ${Qt5Gui_LIBRARIES} ${Qt5Widgets_LIBRARIES})
	add_definitions(-DPOPPLER_QT5)
	add_definitions(-fPIC)
	qt5_wrap_ui(dspdfviewer_UIS_H pdfviewerwindow.ui)
else()
	#qt4
	message(STATUS "Using Qt4 and libpoppler-qt4")
	find_package(Qt4 REQUIRED COMPONENTS QtCore QtGui)
	pkg_search_module(POPPLER REQUIRED poppler-qt4)

	list(APPEND LIST_INCLUDE_DIRS ${QT_INCLUDES})
	list(APPEND LIST_LIBRARIES Qt4::QtGui)
	qt4_wrap_ui(dspdfviewer_UIS_H pdfviewerwindow.ui)
endif()

# include/link poppler
list(APPEND LIST_LIBRARIES ${POPPLER_LIBRARIES})
list(APPEND LIST_INCLUDE_DIRS ${POPPLER_INCLUDE_DIRS})

list(APPEND LIST_INCLUDE_DIRS ${Boost_INCLUDE_DIRS})
list(APPEND LIST_LIBRARIES ${Boost_LIBRARIES})

# include current binary dir, for ui_pdfviewerwindow.h and the .moc files
# Treat this like system headers because it's generated code
list(APPEND LIST_INCLUDE_DIRS ${CMAKE_CURRENT_BINARY_DIR})

# Use c++11 support.
# found on
# http://stackoverflow.com/questions/10984442/how-to-detect-c11-support-of-a-compiler-with-cmake
if(CMAKE_COMPILER_IS_GNUCXX)
	execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
	if (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7)
		message(STATUS "GNU GCC 4.7 or higher detected.")
		add_definitions("-std=c++11")
	elseif(GCC_VERSION VERSION_GREATER 4.3 OR GCC_VERSION VERSION_EQUAL 4.3)
		message(WARNING "You are using an old version of gcc."
			" Consider upgrading to a more recent version if you run into problems.")
		add_definitions("-std=c++0x")
	else ()
		message(FATAL_ERROR "C++11 needed. For GCC that means a version higher than 4.3 is needed.")
	endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	message(STATUS "Clang++ detected.")
	add_definitions("-std=c++11")
else()
	# Unknown Compiler
	message(WARNING "You are using an unidentified and therefore unsupported compiler. "
		" If this works for you, please report that to upstream and - if possible -"
		" please send patches activating sensible compile flags on it.")
	add_definitions(-std=c++11)
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
	#add some warnings
	add_definitions(-Wall -Wextra -pedantic -Werror -Wfatal-errors -Wold-style-cast -Woverloaded-virtual)
	add_definitions(-Weffc++)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	# Turn on a lot of warnings, hopefully helping with code quality.
	add_definitions(-Weverything)
	# FIXME: Re-activate -Werror once these are worked out of the source
	add_definitions(-Werror)

	# Disable warnings irrelevant to the project:
	#   c++98 compatibility warnings
	add_definitions(-Wno-c++98-compat -Wno-c++98-compat-pedantic)
	#   FIXME: Weak vtables
	#   Currently this warning triggers in exception classes
	add_definitions(-Wno-error=weak-vtables)
	#   padding
	add_definitions(-Wno-padded)
	#   in release mode: Unrechable code / Macro expansion (these stem from QDEBUG)
	if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug" )
		add_definitions(-Wno-unreachable-code -Wno-disabled-macro-expansion)
	endif()
	#   Qt's moc (Meta Object Compiler) generates code that triggers warnings
	#   about undefined behaviours.

	#   So don't set Werror for it.
	add_definitions(-Wno-error=undefined-reinterpret-cast)
else()
	message(WARNING "Compiling with a Non-GNU compiler. A lot less warnings will be output, so more coding errors might go undetected.")
endif()

if( CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang" )
	# Include using -isystem
	foreach(lib IN LISTS LIST_INCLUDE_DIRS)
		add_definitions(-isystem ${lib})
	endforeach()
else()
	include_directories(${LIST_INCLUDE_DIRS})
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

set(dspdfviewer_SRCS adjustedlink.cpp hyperlinkarea.cpp pdfpagereference.cpp pdfdocumentreference.cpp runtimeconfiguration.cpp renderutils.cpp renderthread.cpp renderingidentifier.cpp pagepart.cpp renderedpage.cpp pdfrenderfactory.cpp pdfviewerwindow.cpp dspdfviewer.cpp windowrole.cpp)


add_library(libdspdfviewer ${dspdfviewer_SRCS} ${dspdfviewer_UIS_H})
set_target_properties(libdspdfviewer PROPERTIES OUTPUT_NAME dspdfviewer)
target_link_libraries(libdspdfviewer ${LIST_LIBRARIES})

add_executable(dspdfviewer main) #${dspdfviewer_SRCS} ${dspdfviewer_UIS_H})
target_link_libraries(dspdfviewer libdspdfviewer) # ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${Boost_LIBRARIES} ${POPPLER_LIBRARY})


if(BuildTests)
	# Run unit tests regardless of debug/release
	include(CTest)
	add_subdirectory(testing)

	# coverage only works properly with gcc right now.
	if (CMAKE_COMPILER_IS_GNUCXX)
		message(STATUS "Testing with gcc, using gcov as helper")
		add_definitions(-fprofile-arcs -ftest-coverage)
		target_link_libraries(libdspdfviewer gcov)
	endif()
endif()

#### Installation

install(TARGETS	dspdfviewer
	RUNTIME DESTINATION bin)

install(FILES docs/dspdfviewer.1
	DESTINATION share/man/man1)

install(FILES dspdfviewer.desktop
	DESTINATION share/applications)
