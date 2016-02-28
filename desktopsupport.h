#pragma once

#include <memory>
#include <vector>
#include <QWidget>

enum class DesktopEnvironmentHandlerQuality {
	NotMyEnvironment = 0,
	GenericHandler,
	OperatingSpecificHandler,
	DesktopEnvironmentSpecificHandler
};

/** Represents one "output",
 * meaning a physical screen connected to the machine
 *
 * Exact type will be dependent on the handler that created it.
 */
class OutputHandle {
	/** Get a human-readable name of the output */
	virtual std::string const name() const =0;

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

	/** Get all outputs on the machine */
	virtual OutputList getOutputs() const =0;

	/** Get Primary output
	 *
	 * can throw CannotDeterminePrimaryOutput
	 * if there is more than one screen and
	 * none of them is marked primary
	 *
	 * If errors are ignored, will pick the numerically
	 * first screen returned from the environment.
	 */
	virtual OutputPtr getPrimary(
		const DesktopSupportErrorHandling& errorMode=
			DesktopSupportErrorHandling::ThrowOnError
		)
		const =0;

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
	virtual OutputPtr getSecondary(
		const DesktopSupportErrorHandling& errorMode=
			DesktopSupportErrorHandling::ThrowOnError
		)
		const =0;

	/** Move a window to a specified output
	 *
	 * Can throw FailedToMoveToOutput if the desktop environment
	 * signaled an error (this is likely fatal)
	 */
	virtual void moveWindow(QWidget& window, OutputHandle& to)
		const =0;

	/** This is a base class, make destructor virtual */
	virtual ~DesktopSupport() =default;
};
