# CMake file fragment

# File lists

list(APPEND TRANSLATIONFILES
	translations/dspdfviewer_de.ts
)

list(APPEND UIFILES
	pdfviewerwindow.ui
	keybindings.ui
)

list(APPEND dspdfviewer_SRCS
	adjustedlink.cpp
	hyperlinkarea.cpp
	pdfpagereference.cpp
	pdfdocumentreference.cpp
	runtimeconfiguration.cpp
	renderutils.cpp
	renderthread.cpp
	renderingidentifier.cpp
	pagepart.cpp
	renderedpage.cpp
	pdfrenderfactory.cpp
	pdfviewerwindow.cpp
	dspdfviewer.cpp
	windowrole.cpp
)

list(APPEND QRCFILES
	dspdfviewer.qrc
)

list(APPEND TRANSLATIONFILES
	translations/dspdfviewer_de.ts
)
list(APPEND UIFILES
	pdfviewerwindow.ui
	keybindings.ui
)
