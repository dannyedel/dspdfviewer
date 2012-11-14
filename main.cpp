#include <QtGui/QApplication>
#include <QDebug>
#include "dspdfviewer.h"
#include <stdexcept>
#include <QMessageBox>

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	/* If anything goes wrong, try to display the exception to the user.
	 * Its the least i can do.
	 */
	try {
		bool splitPageMode = true;
		QStringList cmdLineArguments = app.arguments();
		
		/** TODO: This is extremely simple command line handling.
		 * It can probably be done a lot better.
		 */
		if ( cmdLineArguments.contains("--full-page") )
		{
		  cmdLineArguments.removeOne("--full-page");
		  splitPageMode=false;
		}
		if ( cmdLineArguments.contains("-f") )
		{
		  cmdLineArguments.removeOne("-f");
		  splitPageMode=false;
		}
		
		if ( cmdLineArguments.size() != 2 )
		{
			throw std::runtime_error("No filename specified on the command line");
		}
		
		/* Register the meta-type so that rendered pages can be passed around
		* using Qt's event/callback system
		*/
		qRegisterMetaType< QSharedPointer<RenderedPage> >("QSharedPointer<RenderedPage>");
		
		DSPDFViewer foo(cmdLineArguments.at(1), splitPageMode);
		
		return app.exec();
	} catch ( std::exception& e ) {
		QMessageBox errorMsg;
		errorMsg.setText("An error has occured.");
		errorMsg.setInformativeText(e.what());
		errorMsg.setDefaultButton(QMessageBox::Discard);
		errorMsg.setIcon( QMessageBox::Critical );
		errorMsg.exec();
		return 1;
	}
}
