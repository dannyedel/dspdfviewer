#pragma once

#include <memory>
#include <vector>
#include <QWidget>

#include "windowrole.h"

enum class DesktopEnvironmentHandlerQuality {
	NotMyEnvironment = 0,
	GenericHandler,
	OperatingSystemSpecificHandler,
	DesktopEnvironmentSpecificHandler
};

/** Represents one "output",
 * meaning a physical screen connected to the machine
 *
 * Exact type will be dependent on the handler that created it.
 */
struct OutputHandle {
	/** Get a human-readable name of the output */
	virtual std::string const name() const =0;

	/** Is this output marked as "primary" ? */
	virtual bool isPrimary() const =0;

	/** Polymorphic class */
	virtual ~OutputHandle() =default;
};
typedef std::unique_ptr<OutputHandle> OutputPtr;
typedef std::vector<OutputPtr> OutputList;

enum class DesktopSupportErrorHandling {
	ThrowOnError,
	TryToIgnoreError
};

/** Abstract base class for desktop support */
class DesktopSupport {
public:
	/** Check handler quality
	 *
	 * This either outputs NotMyEnvironment,
	 * or the quality of the handler
	 *
	 * High quality means it is perfectly suited
	 * for the current execution environment.
	 *
	 */
	virtual DesktopEnvironmentHandlerQuality quality() const =0;

	/** Output the DesktopSupport's name
	 *
	 * Human-readable, but must be unique across implementations.
	 * */
	virtual std::string const name() const =0;

	/** Get all outputs on the machine
	 *
	 * This needs to be implemented by inheriting classes
	 * */
	virtual OutputList getOutputs() const =0;

	/** Get Primary output
	 *
	 * can throw CannotDeterminePrimaryOutput
	 * if there is more than one screen and
	 * none of them is marked primary
	 *
	 * If there is more than one "primary" screen,
	 * it will output the numerically first of them.
	 *
	 * If errors are ignored, will pick the numerically
	 * first screen returned from the environment.
	 */
	OutputPtr getPrimary(
		const DesktopSupportErrorHandling& errorMode=
			DesktopSupportErrorHandling::ThrowOnError
		)
		const;

	/** Get secondary screen
	 *
	 * can throw OnlyOneOutput if there is only
	 * one output
	 *
	 * can throw CannotDeterminePrimaryOutput
	 * if no screeen is marked Primary
	 *
	 * can throw TooManyScreens if there is three
	 * or more screens
	 *
	 * If errors are ignored, will pick the numerically
	 * second one when there is no primary.
	 *
	 * If there is a primary, will pick the numerically
	 * first non-primary.
	 *
	 * The exact screen chosen depends on the environment
	 * and is probably random.
	 */
	OutputPtr getSecondary(
		const DesktopSupportErrorHandling& errorMode=
			DesktopSupportErrorHandling::ThrowOnError
		)
		const;

	/** Move a window to a specified output
	 *
	 * Can throw FailedToMoveToOutput if the desktop environment
	 * signaled an error (this is likely fatal)
	 */
	virtual void moveWindow(QWidget& window, OutputHandle& to)
		const =0;

	/** Get target output for a specific window role */
	OutputPtr getTargetOutput(
		const WindowRole& windowRole,
		const DesktopSupportErrorHandling& errorMode=
			DesktopSupportErrorHandling::ThrowOnError
			) const;

	/** Make the window fullscreen.
	 *
	 * Must not make an already-fullscreened window
	 * "normal" again.
	 *
	 * If the desktop does not allow detection of fullscreen
	 * state, unconditional un-full-screening and re-full-screening
	 * is acceptable as a last resort.
	 */
	virtual void makeFullscreen(QWidget& window) const =0;

	/** This is a base class, make destructor virtual */
	virtual ~DesktopSupport() =default;
};
