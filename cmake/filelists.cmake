# CMake file fragment

# File lists

list(APPEND TRANSLATIONFILES
	translations/dspdfviewer_de.ts
)

list(APPEND UIFILES
	pdfviewerwindow.ui
	keybindings.ui
)

list(APPEND libdspdfviewer_SRCS
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

list(APPEND dspdfviewer_SRCS
	main.cpp
)

list(APPEND QRCFILES
	dspdfviewer.qrc
)

list(APPEND UIFILES
	pdfviewerwindow.ui
	keybindings.ui
)

# Windows file lists:
# Dynamic and static precompiled dependencies

list(APPEND WINDOWS_PRECOMPILED_STATIC_LIBRARIES
	optimized "C:/dspdf/poppler/poppler/lib/poppler.lib"
	debug "C:/dspdf/poppler/poppler/lib/popplerd.lib"
	optimized "C:/dspdf/poppler/poppler/lib/poppler-qt5.lib"
	debug "C:/dspdf/poppler/poppler/lib/poppler-qt5d.lib"
	optimized "C:/dspdf/poppler/deps/cairo/lib/cairo-static.lib"
	debug "C:/dspdf/poppler/deps/cairo/lib/cairo-staticd.lib"
	optimized "C:/dspdf/poppler/deps/freetype/lib/freetype.lib"
	debug "C:/dspdf/poppler/deps/freetype/lib/freetyped.lib"
	optimized "C:/dspdf/poppler/deps/lcms/Lib/MS/lcms2_static.lib"
	debug "C:/dspdf/poppler/deps/lcms/Lib/MS/lcms2_staticd.lib"
	optimized "C:/dspdf/poppler/deps/fontconfig/lib/libfontconfig.lib"
	debug "C:/dspdf/poppler/deps/fontconfig/lib/libfontconfigd.lib"
	optimized "C:/dspdf/poppler/deps/libpng/lib/libpng16_static.lib"
	debug "C:/dspdf/poppler/deps/libpng/lib/libpng16_staticd.lib"
	optimized "C:/dspdf/poppler/deps/libtiff/lib/tiff_static.lib"
	debug "C:/dspdf/poppler/deps/libtiff/lib/tiff_staticd.lib"
	optimized "C:/dspdf/poppler/deps/zlib/lib/zlibstatic.lib"
	debug "C:/dspdf/poppler/deps/zlib/lib/zlibstaticd.lib"
	optimized "C:/dspdf/poppler/deps/expat/lib/expat.lib"
	debug "C:/dspdf/poppler/deps/expat/lib/expatd.lib"
	optimized "C:/dspdf/poppler/deps/openjpeg/lib/openjp2.lib"
	debug "C:/dspdf/poppler/deps/openjpeg/lib/openjp2d.lib"
	optimized "C:/dspdf/poppler/deps/libtiff/lib/port.lib"
	debug "C:/dspdf/poppler/deps/libtiff/lib/portd.lib"
	optimized "C:/dspdf/poppler/deps/libiconv/lib/libiconvStatic.lib"
	debug "C:/dspdf/poppler/deps/libiconv/lib/libiconvStaticD.lib"
	optimized "C:/dspdf/poppler/deps/pixman/lib/pixman-1_static.lib"
	debug "C:/dspdf/poppler/deps/pixman/lib/pixman-1_staticd.lib"
	# Microsoft SDK
	"C:/Program Files (x86)/Microsoft SDKs/Windows/v7.1A/Lib/WS2_32.Lib"
	"C:/Program Files (x86)/Microsoft SDKs/Windows/v7.1A/Lib/OpenGL32.Lib"
	"C:/Program Files (x86)/Microsoft SDKs/Windows/v7.1A/Lib/MSImg32.Lib"
	"C:/Program Files (x86)/Microsoft SDKs/Windows/v7.1A/Lib/Imm32.Lib"
	"C:/Program Files (x86)/Microsoft SDKs/Windows/v7.1A/Lib/Winmm.Lib"
)

list(APPEND WINDOWS_PRECOMPILED_DYNAMIC_LIBRARIES
	optimized "C:/dspdf/popplerDyn/poppler/lib/poppler.lib"
	debug "C:/dspdf/popplerDyn/poppler/lib/popplerd.lib"
	optimized "C:/dspdf/popplerDyn/poppler/lib/poppler-qt5.lib"
	debug "C:/dspdf/popplerDyn/poppler/lib/poppler-qt5d.lib"
	optimized "C:/dspdf/popplerDyn/deps/cairo/lib/cairo.lib"
	debug "C:/dspdf/popplerDyn/deps/cairo/lib/cairod.lib"
	optimized "C:/dspdf/popplerDyn/deps/freetype/lib/freetype.lib"
	debug "C:/dspdf/popplerDyn/deps/freetype/lib/freetyped.lib"
	optimized "C:/dspdf/popplerDyn/deps/lcms/Lib/MS/lcms2.lib"
	debug "C:/dspdf/popplerDyn/deps/lcms/Lib/MS/lcms2d.lib"
	optimized "C:/dspdf/popplerDyn/deps/fontconfig/lib/fontconfig.lib"
	debug "C:/dspdf/popplerDyn/deps/fontconfig/lib/fontconfigd.lib"
	optimized "C:/dspdf/popplerDyn/deps/libpng/lib/libpng16.lib"
	debug "C:/dspdf/popplerDyn/deps/libpng/lib/libpng16d.lib"
	optimized "C:/dspdf/popplerDyn/deps/libtiff/lib/tiff.lib"
	debug "C:/dspdf/popplerDyn/deps/libtiff/lib/tiffd.lib"
	optimized "C:/dspdf/popplerDyn/deps/zlib/lib/zlib.lib"
	debug "C:/dspdf/popplerDyn/deps/zlib/lib/zlibd.lib"
	"C:/dspdf/popplerDyn/deps/expat/lib/expat.lib"
	optimized "C:/dspdf/popplerDyn/deps/openjpeg/lib/openjpeg.lib"
	debug "C:/dspdf/popplerDyn/deps/openjpeg/lib/openjpegd.lib"
	optimized "C:/dspdf/popplerDyn/deps/libiconv/lib/libiconv.lib"
	debug "C:/dspdf/popplerDyn/deps/libiconv/lib/libiconvD.lib"
	optimized "C:/dspdf/popplerDyn/deps/pixman/lib/pixman-1_static.lib"
	debug "C:/dspdf/popplerDyn/deps/pixman/lib/pixman-1_staticd.lib"
)
