project(dspdfviewer)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

add_definitions(-std=c++0x)

set(dspdfviewer_SRCS pdfviewerwindow.cpp primarywindow.cpp secondarywindow.cpp dspdfviewer.cpp main.cpp)
qt4_automoc(${dspdfviewer_SRCS})
add_executable(dspdfviewer ${dspdfviewer_SRCS})
target_link_libraries(dspdfviewer ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} poppler-qt4)
