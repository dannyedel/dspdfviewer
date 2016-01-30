#pragma once

#include "dspdfviewer.h"

/** Test driver
 *
 * If it detects two screens, it will verify that the windows switch to the
 * specified positions.
 * If running on single-screen, the commands will be executed without verification,
 * just to make sure the code doesn't segfault or similar.
 *
 * Rinse and repeat 3 times.
 *
 */
class SwapScreensAndCheckAlign: public QObject {
	Q_OBJECT

	DSPDFViewer& dspdfviewer;
	QRect screenPrimary;
	QRect screenSecondary;
	bool verify;
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
