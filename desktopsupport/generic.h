#pragma once

#include "../desktopsupport.h"

#include <QApplication>
#include <QRect>

/** On the Generic Handler, an "output handle"
 * is just a rectangular place on the output
 */
struct GenericOutputHandle: public OutputHandle {
	QRect screenRect;
	bool primary;

	inline bool isPrimary() const override{
		return primary;
	}

	GenericOutputHandle(const QRect& rect, bool b):
		screenRect(rect), primary(b) {
	}

	const std::string name() const override;
};

class GenericDesktopSupport: public DesktopSupport {

	DesktopEnvironmentHandlerQuality quality() const override {
		return DesktopEnvironmentHandlerQuality::GenericHandler;
	}

	std::string const name() const override {
		return "Generic Qt4/5 desktop support handler";
	}

	OutputList getOutputs() const override;

	void moveWindow(QWidget&, OutputHandle&) const override;

	void makeFullscreen(QWidget&) const override;
	void removeFullscreen(QWidget&) const override;
};
