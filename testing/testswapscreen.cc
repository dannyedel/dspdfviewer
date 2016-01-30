#include "testswapscreen.hh"

#include <QDesktopWidget>

using namespace std;
using namespace TestHelpers;

SwapScreensAndCheckAlign::SwapScreensAndCheckAlign(DSPDFViewer& d):
	dspdfviewer(d),
	screenPrimary( QApplication::desktop()->screenGeometry( 0 ) ),
	screenSecondary( QApplication::desktop()->screenGeometry( 1 ) ),
	verify(true)
{
	DEBUGOUT << "Number of screens:" << QApplication::desktop()->numScreens() ;

	DEBUGOUT << "screen 0 [pri]:" << screenPrimary;
	DEBUGOUT << "screen 1 [sec]:" << screenSecondary;


	if ( QApplication::desktop()->numScreens() != 2 ) {
		WARNINGOUT << "Not running in a dual-screen environment";
		verify = false;
	} else if ( screenPrimary == screenSecondary ) {
		WARNINGOUT << "Cannot tell the screens apart";
		verify = false;
	} else if ( screenPrimary.width() == 0 || screenPrimary.height() == 0 ) {
		WARNINGOUT << "Cannot query primary screen size";
		verify = false;
	} else if ( screenSecondary.width() == 0 || screenSecondary.height() == 0 ) {
		WARNINGOUT << "Cannot query secondary screen size";
		verify = false;
	}
	if ( ! verify ) {
		WARNINGOUT << "Disabling verification.";
	}
	// 1 Second time to boot
	QTimer::singleShot(1000, this, SLOT(checkStartPositions()));
}

void SwapScreensAndCheckAlign::checkStartPositions() {
	DEBUGOUT << "Start positions";
	if ( verify ) {
		check(dspdfviewer.audienceGeometry(), screenSecondary); // Audience starts on external
		check(dspdfviewer.secondGeometry(), screenPrimary); // Presenter starts on internal
	}
	DEBUGOUT << "Firing screen swap event";
	emit screenSwapRequested();
	// 0.5 sec to swap
	QTimer::singleShot(500, this, SLOT(checkAfterFirstSwap()));
}

void SwapScreensAndCheckAlign::checkAfterFirstSwap() {
	DEBUGOUT << "Check after first swap";
	if ( verify ) {
		check(dspdfviewer.audienceGeometry(), screenPrimary); // Audience window on laptop
		check(dspdfviewer.secondGeometry(), screenSecondary); // Presenter window on beamer
	}
	emit screenSwapRequested();
	// 0.5 sec time to swap
	QTimer::singleShot(500, this, SLOT(checkAfterSwapBack()));
}

void SwapScreensAndCheckAlign::checkAfterSwapBack() {
	DEBUGOUT << "Check after swapping back";
	if ( verify ) {
		check(dspdfviewer.audienceGeometry(), screenSecondary);
		check(dspdfviewer.secondGeometry(), screenPrimary);
	}
	emit quitRequested();
}

/** Run dspdfviewer, checking if screens correctly re-position to the monitors */
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
