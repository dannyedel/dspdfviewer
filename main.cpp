/*
    dspdfviewer - Dual Screen PDF Viewer for LaTeX-Beamer
    Copyright (C) 2012  Danny Edel <mail@danny-edel.de>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#include <QApplication>
#include "debug.h"
#include "dspdfviewer.h"
#include "runtimeconfiguration.h"
#include <stdexcept>
#include <iostream>
#include <QMessageBox>
#include <QFileDialog>
#include <QTranslator>
#include <QLibraryInfo>

#if defined ( _WIN32 )
	#pragma comment(linker, "/ENTRY:mainCRTStartup")

	#if defined( NDEBUG )
		#pragma comment(linker, "/SUBSYSTEM:windows")
	#endif

#endif

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	app.setApplicationName( QString::fromUtf8("dspdfviewer") );
	app.setApplicationVersion( QString::fromUtf8( DSPDFVIEWER_VERSION ) );

	const auto locale = QLocale::system();
	const auto localeName = locale.name();
	const auto systemTranslationsPath =
		QLibraryInfo::location(QLibraryInfo::TranslationsPath);

	QTranslator qtTranslator;
	DEBUGOUT << "Loading qt system translation for" << localeName
		<< "from" << systemTranslationsPath;
	if ( !qtTranslator.load(
			QString::fromUtf8( "qt_" ) + localeName, systemTranslationsPath )
			) {
		WARNINGOUT << "Failed to load qt translations for locale" << localeName;
	} else {
		app.installTranslator(&qtTranslator);
		DEBUGOUT << "Qt system translation loaded for" << localeName;
	}

	QTranslator appTranslator;
	DEBUGOUT << "Loading dspdfviewer translation for current locale:" << localeName;
	if ( ! appTranslator.load(QString::fromUtf8(":/translations/dspdfviewer") ) ) {
		WARNINGOUT << "Failed to load dspdfviewer translation for current locale" << localeName;
	} else {
		app.installTranslator(&appTranslator);
		DEBUGOUT << "dspdfviewer translation loaded for" << localeName;
	}

	/* If anything goes wrong, try to display the exception to the user.
	 * Its the least i can do.
	 */
	try {
		/* Register the meta-type so that rendered pages can be passed around
		* using Qt's event/callback system
		*/
		qRegisterMetaType< QSharedPointer<RenderedPage> >("QSharedPointer<RenderedPage>");
		RuntimeConfiguration rc(argc, argv);

		if ( ! rc.filePathDefined() ) {
			rc.filePath( qPrintable( QFileDialog::getOpenFileName(
				nullptr, QFileDialog::tr("Load PDF from disk"), QString(), QFileDialog::tr("PDF (*.pdf)") ) ) );
		}

		DSPDFViewer foo( rc );
		return app.exec();
	} catch ( std::exception& e ) {
		std::cerr << "----- FATAL ERROR -----" << std::endl
			<< qPrintable( QCoreApplication::translate("DSPDFViewer", "Dual-Screen PDF Viewer has encountered an error and cannot continue") ) << std::endl
			<< e.what() << std::endl;

		QMessageBox errorMsg;
		errorMsg.setText( QCoreApplication::translate("DSPDFViewer", "Dual-Screen PDF Viewer has encountered an error and cannot continue") );
		errorMsg.setInformativeText( QString::fromLocal8Bit( e.what() ) );
		errorMsg.setDefaultButton(QMessageBox::Discard);
		errorMsg.setIcon( QMessageBox::Critical );
		errorMsg.exec();
		return 1;
	}
}
