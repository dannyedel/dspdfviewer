# cmake frament

# External libraries


### Boost Libraries ###

if( BoostStaticLink )
	set(Boost_USE_STATIC_LIBS ON)
	if(WindowsStaticLink)
		set(Boost_USE_STATIC_RUNTIME ON)
	endif()
elseif(BuildTests)
	add_definitions(-DBOOST_TEST_DYN_LINK)
endif()

if(BuildTests)
	# If we're building tests, we need program_options
	# AND unit_test_framework
	find_package(Boost
		COMPONENTS program_options unit_test_framework
		REQUIRED)
else()
	# No unit tests: Only program_options needed
	find_package(Boost
		COMPONENTS program_options
		REQUIRED)
endif()


### Qt
if(UseQtFive)
	# Qt5
	message(STATUS "Using Qt5 and libpoppler-qt5")
	if(MSVC AND WindowsStaticLink)
		# If linking statically,
		# Try to find Qt in /qt/static/qtbase
		# or plain /qt/static
		set(CMAKE_PREFIX_PATH
			"/Qt/static/qtbase;/Qt/static;/Qt/static/qttools;${CMAKE_PREFIX_PATH}")
	endif()
	find_package(Qt5Core 5.1.1 REQUIRED)
	# On lower versions, QTBUG-32100 prevents compilation at least on clang
	# https://bugreports.qt.io/browse/QTBUG-32100
	find_package(Qt5Gui REQUIRED)
	find_package(Qt5Widgets REQUIRED)
	find_package(Qt5LinguistTools REQUIRED)
	find_package(Qt5Xml REQUIRED)
	message(STATUS "Found Qt5 at ${Qt5Core_DIR}")

	if(MSVC)
		if (WindowsStaticLink)
			set(POPPLER_LIBRARIES
				${WINDOWS_PRECOMPILED_STATIC_LIBRARIES}
			)
			set(POPPLER_INCLUDE_DIRS
				"C:/dspdf/poppler/poppler/include/poppler/qt5"
			)
		else() # MSVC, but not statically linking
			set(POPPLER_LIBRARIES
				${WINDOWS_PRECOMPILED_DYNAMIC_LIBRARIES}
			)
			set(POPPLER_INCLUDE_DIRS
				"C:/dspdf/popplerDyn/poppler/include/poppler/qt5"
			)
		endif()
	else()
		# Non-MSVC Platforms:
		# Find poppler using pkg-config
		pkg_search_module(POPPLER REQUIRED poppler-qt5)
	endif()

	# add their link flags
	list(APPEND LIST_LIBRARIES
		${Qt5Core_LIBRARIES}
		${Qt5Gui_LIBRARIES}
		${Qt5Widgets_LIBRARIES}
		${Qt5LinguistTools_LIBRARIES}
		${Qt5Xml_LIBRARIES}
	)

	# add their include directories
	list(APPEND LIST_INCLUDE_DIRS
		${Qt5Core_INCLUDE_DIRS}
		${Qt5Gui_INCLUDE_DIRS}
		${Qt5Widgets_INCLUDE_DIRS}
		${Qt5LinguistTools_INCLUDE_DIRS}
		${Qt5Xml_INCLUDE_DIRS}
	)

	# Set conditional compilation to Qt5 mode
	add_definitions(-DPOPPLER_QT5)
	qt5_wrap_ui(dspdfviewer_UIS_H ${UIFILES})
	if( UpdateTranslations )
		qt5_create_translation(TRANSLATIONS
			${libdspdfviewer_SRCS} ${dspdfviewer_SRCS} ${UIFILES} ${TRANSLATIONFILES}
			OPTIONS -no-obsolete
			)
	else()
		qt5_add_translation(TRANSLATIONS ${TRANSLATIONFILES})
	endif()
else()
	#qt4
	message(STATUS "Using Qt4 and libpoppler-qt4")
	message(WARNING "Qt4 support is deprecated and will be removed in the next version. "
		"Please build dspdfviewer using Qt5. "
		"If that does not work correctly on your platform, please file a bug!")
	find_package(Qt4 REQUIRED COMPONENTS QtCore QtGui)
	pkg_search_module(POPPLER REQUIRED poppler-qt4)

	list(APPEND LIST_INCLUDE_DIRS ${QT_INCLUDES})
	list(APPEND LIST_LIBRARIES Qt4::QtGui)
	qt4_wrap_ui(dspdfviewer_UIS_H ${UIFILES})
	if( UpdateTranslations )
		qt4_create_translation(TRANSLATIONS
			${libdspdfviewer_SRCS} ${dspdfviewer_SRCS} ${UIFILES} ${TRANSLATIONFILES}
			OPTIONS -no-obsolete
			)
	else()
		qt4_add_translation(TRANSLATIONS ${TRANSLATIONFILES})
	endif()
endif()

# include/link poppler
list(APPEND LIST_LIBRARIES ${POPPLER_LIBRARIES})
list(APPEND LIST_INCLUDE_DIRS ${POPPLER_INCLUDE_DIRS})

list(APPEND LIST_INCLUDE_DIRS ${Boost_INCLUDE_DIRS})
list(APPEND LIST_LIBRARIES ${Boost_PROGRAM_OPTIONS_LIBRARY})
if(BuildTests)
	list(APPEND LIST_TEST_LIBRARIES ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
endif()

# This is needed for moc/uic results
list(APPEND LIST_INCLUDE_DIRS ${CMAKE_CURRENT_BINARY_DIR})
