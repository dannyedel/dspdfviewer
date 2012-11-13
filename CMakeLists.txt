project(dspdfviewer)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

add_definitions(-ggdb -std=c++11)
#add some warnings
add_definitions(-Wall -Wextra -pedantic -Werror -ggdb -Wfatal-errors -Wold-style-cast -Woverloaded-virtual)

#I like these for my code, but Qt doesnt compile with them
# add_definitions( -Weffc++ )
add_definitions(-Wno-effc++)

set(dspdfviewer_SRCS renderutils.cpp renderthread.cpp renderingidentifier.cpp pagepart.cpp renderedpage.cpp pdfrenderfactory.cpp pdfviewerwindow.cpp dspdfviewer.cpp main.cpp)
qt4_automoc(${dspdfviewer_SRCS})
add_executable(dspdfviewer ${dspdfviewer_SRCS})
target_link_libraries(dspdfviewer ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} poppler-qt4)

install(TARGETS	dspdfviewer
	RUNTIME DESTINATION bin)
