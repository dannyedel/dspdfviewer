#include "../desktopsupport.h"

/** In the win32 handler, we "memorize" the screens
 * by their index in
 * QApplication::screens()
 */
struct Win32OutputHandle: OutputHandle{
	int screenNumber;

	inline bool isPrimary() const override {
		return screenNumber == 0;
	}

	Win32OutputHandle(int i):
		screenNumber(i)
	{
	}

	const std::string name() const override;
};

class Win32DesktopSupport: public DesktopSupport {
	DesktopEnvironmentHandlerQuality quality() const override;
	inline std::string const name() const override {
		return "Win32 Desktop Support";
	}
	OutputList getOutputs() const override;
	void moveWindow(QWidget&, OutputHandle&) const override;
	bool canMoveWindowWhileFullscreen() const override {
		return false;
	}
};
