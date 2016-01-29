#include "testswapscreen.hh"

#include <QDesktopWidget>

using namespace std;
using namespace TestHelpers;

SwapScreensAndCheckAlign::SwapScreensAndCheckAlign(DSPDFViewer& d):
	dspdfviewer(d),
	screenPrimary( QApplication::desktop()->screenGeometry( 0 ) ),
	screenSecondary( QApplication::desktop()->screenGeometry( 1 ) )
{

	DEBUGOUT << "Number of screens:" << QApplication::desktop()->numScreens() ;

	DEBUGOUT << "screen 0 [pri]:" << screenPrimary;
	DEBUGOUT << "screen 1 [sec]:" << screenSecondary;

	if ( screenPrimary.size() == screenSecondary.size() ) {
		WARNINGOUT << "Cannot tell the screens apart! Are you running this test in a dualscreen environment?";
		std::exit( 125 );
	}
	else if ( screenPrimary.width() == 0 || screenPrimary.height() == 0 ) {
		WARNINGOUT << "Cannot query primary screen size!";
		std::exit( 125 );
	}
	else if ( screenSecondary.width() == 0 || screenSecondary.height() == 0 ) {
		WARNINGOUT << "Cannot query secondary screen size!";
		std::exit( 125 );
	}
	else {
		// 1 Second time to boot
		QTimer::singleShot(1000, this, SLOT(checkStartPositions()));
	}
}

void SwapScreensAndCheckAlign::checkStartPositions() {
	DEBUGOUT << "Start positions";
	check(dspdfviewer.audienceGeometry().size(), screenSecondary.size()); // Audience starts on external
	check(dspdfviewer.secondGeometry().size(), screenPrimary.size()); // Presenter starts on internal
	DEBUGOUT << "Firing screen swap event";
	emit screenSwapRequested();
	// 0.5 sec to swap
	QTimer::singleShot(500, this, SLOT(checkAfterFirstSwap()));
}

void SwapScreensAndCheckAlign::checkAfterFirstSwap() {
	DEBUGOUT << "Check after first swap";
	check(dspdfviewer.audienceGeometry().size(), screenPrimary.size()); // Audience window on laptop
	check(dspdfviewer.secondGeometry().size(), screenSecondary.size()); // Presenter window on beamer
	emit screenSwapRequested();
	// 0.5 sec time to swap
	QTimer::singleShot(500, this, SLOT(checkAfterSwapBack()));
}

void SwapScreensAndCheckAlign::checkAfterSwapBack() {
	DEBUGOUT << "Check after swapping back";
	check(dspdfviewer.audienceGeometry().size(), screenSecondary.size());
	check(dspdfviewer.secondGeometry().size(), screenPrimary.size());
	emit quitRequested();
}

/** Run dspdfviewer, checking if screens correctly re-position to the monitors */
int main(int argc, char** argv) {

	QApplication qapp(argc,argv);

	qRegisterMetaType< QSharedPointer<RenderedPage> >("QSharedPointer<RenderedPage>");

	const char * rcargv[] = {
		"./dspdfviewer",
		"--prerender-previous=0",
		"--prerender-next=0",
		"images.pdf"
	};
	const int rcargc=4;

	RuntimeConfiguration rc{rcargc, rcargv};

	DSPDFViewer dsp{rc};
	SwapScreensAndCheckAlign testdriver{dsp};

	sconnect(&testdriver, SIGNAL(screenSwapRequested()), &dsp, SLOT(swapScreens()));
	sconnect(&testdriver, SIGNAL(quitRequested()), &dsp, SLOT(exit()));

	return qapp.exec();
}
