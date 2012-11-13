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
		if ( app.arguments().size() < 2 )
		{
			throw std::runtime_error("No filename specified on the command line");
		}
		
		/* Register the meta-type so that rendered pages can be passed around
		* using Qt's event/callback system
		*/
		qRegisterMetaType< QSharedPointer<RenderedPage> >("QSharedPointer<RenderedPage>");
		
		DSPDFViewer foo(app.arguments().at(1));
		
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
