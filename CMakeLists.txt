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
cmake_minimum_required(VERSION 2.8.4 FATAL_ERROR)
if( NOT CMAKE_VERSION VERSION_LESS 2.8.11)
	cmake_policy(SET CMP0020 NEW) # qtmain.lib linkage on windows
endif()
if( NOT CMAKE_VERSION VERSION_LESS 3.1)
	cmake_policy(SET CMP0054 NEW) # dont expand quoted strings in if()s
endif()
if( NOT CMAKE_VERSION VERSION_LESS 3.10)
	cmake_policy(SET CMP0071 NEW) # Also process generated source files with MOC
endif()
if( NOT CMAKE_VERSION VERSION_LESS 3.17)
	cmake_policy(SET CMP0100 NEW) # Allow .hh extension
endif()

project(dspdfviewer)

include(FindPkgConfig)
set(CMAKE_AUTOMOC ON)#
set(CMAKE_AUTORCC OFF)
set(CMAKE_INCLUDE_CURRENT_DIR OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(UpdateTranslations "Do you want to update the .ts files (WARNING: running make clean will delete them!)" OFF)
option(BuildTests "Build unit tests (this requires pdflatex or internet access and DownloadTestPDFs=ON)" ON)
option(RunDualScreenTests "Also run tests that require two screens to be connected" ON)
option(RunTestsOnBigEndian "Run tests on a big-endian system" OFF)
option(BoostStaticLink "Link statically against the boost libraries" OFF)
option(WindowsStaticLink "Windows/MSVC only: Link statically against the dependencies and set /MT instead of /MD" ON)
option(UsePrerenderedPDF "Use prerendered PDFs included in the source for testing, instead of building with pdflatex" OFF)
option(CodeCoverage "Add coverage analysis code to the program. Will slow it down. A lot. Only supported on GCC right now." OFF)

include(cmake/platforms.cmake)
include(cmake/filelists.cmake)
include(cmake/external_libraries.cmake)
include(cmake/qrc_embedding.cmake)
include(cmake/compilers.cmake)
include(cmake/version_number.cmake)


if( I3WORKAROUND_SHELLCODE )
	add_definitions(-DI3WORKAROUND_SHELLCODE="${I3WORKAROUND_SHELLCODE}")
endif()

include_directories(SYSTEM ${LIST_INCLUDE_DIRS})

### Compile all the source code into one .a file

add_library(libdspdfviewer ${libdspdfviewer_SRCS} ${dspdfviewer_UIS_H} ${TRANSLATIONS})
# Set target name to "libdspdfviewer"
# without a prefix (normally unix would auto-add lib...)
set_target_properties(libdspdfviewer PROPERTIES
	PREFIX ""
	)
target_link_libraries(libdspdfviewer ${LIST_LIBRARIES})

### And link it together with main.cpp to produce an executable
add_executable(dspdfviewer ${dspdfviewer_SRCS} ${EMBEDDED_QRC})
target_link_libraries(dspdfviewer libdspdfviewer)

### ... and link it with the tests to produce a testing executable.
if(BuildTests)
	# Run unit tests regardless of debug/release
	# Set a default timeout to 60 seconds
	set(DART_TESTING_TIMEOUT 60)
	set(CTEST_TEST_TIMEOUT 60)
	# Check for big endian
	include(TestBigEndian)
	TEST_BIG_ENDIAN(BigEndian)
	if( NOT BigEndian OR RunTestsOnBigEndian)
		include(CTest)
		add_subdirectory(testing)
	else()
		message(WARNING "The unit tests have been temporarily disabled on big-endian "
			"systems.  If you want to help in debugging this, please pass "
			"-DRunTestsOnBigEndian=ON to cmake to force their execution.")
	endif()
endif()

#### Installation

install(TARGETS	dspdfviewer
	RUNTIME DESTINATION bin)

install(FILES dspdfviewer.1
	DESTINATION share/man/man1)

install(FILES dspdfviewer.desktop
	DESTINATION share/applications)
