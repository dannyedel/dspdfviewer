#include "testswapscreen.hh"
#include "testhelpers.hh"

int main(int argc, char** argv) {

	QApplication qapp(argc,argv);

	qRegisterMetaType< QSharedPointer<RenderedPage> >("QSharedPointer<RenderedPage>");

	RuntimeConfiguration rc{argc, argv};

	DSPDFViewer dsp{rc};
	SwapScreensAndCheckAlign testdriver{dsp};

	sconnect(&testdriver, SIGNAL(screenSwapRequested()), &dsp, SLOT(swapScreens()));
	sconnect(&testdriver, SIGNAL(quitRequested()), &dsp, SLOT(exit()));

	return qapp.exec();
}
