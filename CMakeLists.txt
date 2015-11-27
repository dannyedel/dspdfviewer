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
cmake_policy(SET CMP0020 NEW) # qtmain.lib linkage on windows
cmake_policy(SET CMP0054 NEW) # dont expand quoted strings in if()s

project(dspdfviewer)

include(FindPkgConfig)
set(CMAKE_AUTOMOC ON)#
set(CMAKE_AUTORCC OFF)
set(CMAKE_INCLUDE_CURRENT_DIR OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(UseQtFive "Build with Qt5 and libpoppler-qt5" OFF)
option(UpdateTranslations "Do you want to update the .ts files (WARNING: running make clean will delete them!)" OFF)
option(BuildTests "Build unit tests (this requires pdflatex or internet access and DownloadTestPDFs=ON)" ON)
option(BoostStaticLink "Link statically against the boost libraries" OFF)
option(DownloadTestPDF "Download test PDFs from http://danny-edel.de, rather then building them using latex-beamer" OFF)
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
	include(CTest)
	add_subdirectory(testing)
endif()

#### Installation

install(TARGETS	dspdfviewer
	RUNTIME DESTINATION bin)

install(FILES docs/dspdfviewer.1
	DESTINATION share/man/man1)

install(FILES dspdfviewer.desktop
	DESTINATION share/applications)
