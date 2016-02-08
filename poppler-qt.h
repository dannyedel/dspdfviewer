#ifndef POPPLER_QT5
#include <poppler-qt4.h>
#else
#include <poppler-qt5.h>

#if defined( WINDOWS_STATIC_LINK) and not defined(qtplugin)
  #define qtplugin
  #if defined(WIN32) || defined(WIN64)
    #include <QtPlugin>
    Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
  #endif
#endif

#endif
