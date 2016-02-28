#pragma once

/** Include all headers to access the dspdfviewer objects */
#include "../adjustedlink.h"
#include "../debug.h"
#include "../dspdfviewer.h"
#include "../hyperlinkarea.h"
#include "../pagepart.h"
#include "../pdfcacheoption.h"
#include "../pdfdocumentreference.h"
#include "../pdfpagereference.h"
#include "../pdfrenderfactory.h"
#include "../pdfviewerwindow.h"
#include "../renderedpage.h"
#include "../renderingidentifier.h"
#include "../renderthread.h"
#include "../renderutils.h"
#include "../runtimeconfiguration.h"
#include "../sconnect.h"
#include "../windowrole.h"

#include <chrono>
#include <iostream>

#include <boost/test/unit_test.hpp>

namespace TestHelpers {

	/** Returns full path to the specified file */
	QString pdfFilename( const std::string& basename );

	/** Handle to a integration test environment.
	 * Spawns an in-memory X-Server in the background and allows to send
	 * keystrokes and inspect a certain pixel's color.
	 */
	class IntegrationTestEnv {
	public:
		/**
		 * Starts a dummy X server and loads dspdfviewer with the
		 * given command line arguments.
		 */
		IntegrationTestEnv( const std::vector<std::string> & args);

		/** Send a keystroke and wait for a specified time
		 *
		 * See Qt::Key for the list of keys
		 */
		void sendKeyAndWait( const int& key, const std::chrono::milliseconds& t);

		/** Inspect pixel color */
		QColor grabPixelColor( uint x, uint y);
	};

	template<typename T1, typename T2>
		inline
		void check(const T1& lhs, const T2& rhs, int failcode=2) {
			if ( ! (lhs == rhs) ) {
				WARNINGOUT << "Failure:" << lhs << "!=" << rhs;
				QApplication::exit(failcode);
			} else {
				DEBUGOUT << "All right." << lhs << "==" << rhs;
			}
		}
}

/** Print a QSize to a standard output stream */
std::ostream& operator << (std::ostream& where, const QSize& what);

/** Print a QColor to a standard output stream */
std::ostream& operator << (std::ostream& where, const QColor& what);
