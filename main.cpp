#include <QtGui/QApplication>
#include <QDebug>
#include "dspdfviewer.h"
#include "runtimeconfiguration.h"
#include <stdexcept>
#include <QMessageBox>

int main(int argc, char** argv)
{
	RuntimeConfiguration rc;
	QApplication app(argc, argv);
	/* If anything goes wrong, try to display the exception to the user.
	 * Its the least i can do.
	 */
	try {
		/* Register the meta-type so that rendered pages can be passed around
		* using Qt's event/callback system
		*/
		qRegisterMetaType< QSharedPointer<RenderedPage> >("QSharedPointer<RenderedPage>");
		
		rc.parse(argc, argv);
		
		DSPDFViewer foo( rc.filePathQString(), rc.useFullPage());
		
		return app.exec();
	} catch ( std::exception& e ) {
		QMessageBox errorMsg;
		errorMsg.setText("Dual-Screen PDF Viewer has encountered an error and cannot continue");
		errorMsg.setInformativeText(e.what());
		errorMsg.setDefaultButton(QMessageBox::Discard);
		errorMsg.setIcon( QMessageBox::Critical );
		errorMsg.exec();
		return 1;
	}
}
