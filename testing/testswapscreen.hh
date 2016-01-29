#pragma once
#include "testhelpers.hh"

/** Test driver
 *
 * Fails with 125 when there is only one screen detected!
 *
 * First checks whether the windows align to the screens within 1.0 sec,
 * then issues the swapScreen command and checks they settle within another 0.5 sec.
 *
 * Rinse and repeat 3 times.
 *
 */
class SwapScreensAndCheckAlign: public QObject {
	Q_OBJECT
	DSPDFViewer& dspdfviewer;
	QRect screenPrimary;
	QRect screenSecondary;
public:
	SwapScreensAndCheckAlign(DSPDFViewer& app);
signals:
	void screenSwapRequested();
	void quitRequested();
public slots:
	void checkStartPositions();
	void checkAfterFirstSwap();
	void checkAfterSwapBack();
};
