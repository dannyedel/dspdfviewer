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

#if defined ( _WIN32 ) && defined ( NDEBUG )
#pragma comment(linker, "/SUBSYSTEM:windows")
#include <Windows.h>

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	int argc = __argc;
	#define argv __argv

#else

int main(int argc, char** argv)
{

#endif
	QApplication app(argc, argv);
	app.setApplicationName( "dspdfviewer" );
	app.setApplicationVersion( DSPDFVIEWER_VERSION );
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
			rc.filePath( QFileDialog::getOpenFileName(
				nullptr, "Load PDF from disk", QString(), "PDF (*.pdf)" ).toStdString() );
		}

		DSPDFViewer foo( rc );
		return app.exec();
	} catch ( std::exception& e ) {
		QMessageBox errorMsg;
		std::cerr << "----- FATAL ERROR -----" << std::endl
			<< "Dual-Screen PDF Viewer has encountered an error and cannot continue:" << std::endl
			<< e.what() << std::endl;

		errorMsg.setText("Dual-Screen PDF Viewer has encountered an error and cannot continue");
		errorMsg.setInformativeText(e.what());
		errorMsg.setDefaultButton(QMessageBox::Discard);
		errorMsg.setIcon( QMessageBox::Critical );
		errorMsg.exec();
		return 1;
	}
}
