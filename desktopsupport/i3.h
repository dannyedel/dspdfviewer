#pragma once

#include "../desktopsupport.h"

struct i3OutputHandle: public OutputHandle {
	const std::string xrandrName;
	const std::string name() const override;
	const bool primary;
	bool isPrimary() const { return primary; }

	i3OutputHandle(const std::string&, bool);
};

class i3DesktopSupport: public DesktopSupport {
	DesktopEnvironmentHandlerQuality quality() const override;
	std::string const name() const override {
		return "i3 desktop support via i3 IPC";
	}
	OutputList getOutputs() const override;
	void moveWindow(QWidget&, OutputHandle&) const override;
	void makeFullscreen(QWidget&) const override;
private:
	static const std::string getSocketpath();
};
