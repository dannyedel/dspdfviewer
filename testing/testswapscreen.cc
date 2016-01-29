#include "testswapscreen.hh"

#include <QDesktopWidget>

using namespace std;
using namespace TestHelpers;

SwapScreensAndCheckAlign::SwapScreensAndCheckAlign(DSPDFViewer& d):
	dspdfviewer(d),
	screenPrimary( QApplication::desktop()->screenGeometry( 0 ) ),
	screenSecondary( QApplication::desktop()->screenGeometry( 1 ) )
{
	DEBUGOUT << "screen 0 [pri]:" << screenPrimary;
	DEBUGOUT << "screen 1 [sec]:" << screenSecondary;

	if ( screenPrimary == screenSecondary ) {
		WARNINGOUT << "Cannot tell the screens apart! Are you running this test in a dualscreen environment?";
		QApplication::exit( 125);
	}

	// 1 Second time to boot
	QTimer::singleShot(1000, this, SLOT(checkStartPositions()));
}

#ifndef WARNINGOUT
#define WARNINGOUT qWarning()
#endif

void SwapScreensAndCheckAlign::checkStartPositions() {
	DEBUGOUT << "Start positions";
	check(dspdfviewer.audienceGeometry(), screenSecondary); // Audience starts on external
	check(dspdfviewer.secondGeometry(), screenPrimary); // Presenter starts on internal
	DEBUGOUT << "Firing screen swap event";
	emit screenSwapRequested();
	// 0.5 sec to swap
	QTimer::singleShot(500, this, SLOT(checkAfterFirstSwap()));
}

void SwapScreensAndCheckAlign::checkAfterFirstSwap() {
	DEBUGOUT << "Check after first swap";
	check(dspdfviewer.audienceGeometry(), screenPrimary); // Audience window on laptop
	check(dspdfviewer.secondGeometry(), screenSecondary); // Presenter window on beamer
	emit screenSwapRequested();
	// 0.5 sec time to swap
	QTimer::singleShot(500, this, SLOT(checkAfterSwapBack()));
}

void SwapScreensAndCheckAlign::checkAfterSwapBack() {
	DEBUGOUT << "Check after swapping back";
	check(dspdfviewer.audienceGeometry(), screenSecondary);
	check(dspdfviewer.secondGeometry(), screenPrimary);
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
