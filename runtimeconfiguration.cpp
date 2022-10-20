/*
    dspdfviewer - Dual Screen PDF Viewer for LaTeX-Beamer
    Copyright (C) 2012  Danny Edel <mail@danny-edel.de>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#include "runtimeconfiguration.h"

#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
#include <QCoreApplication>

#ifndef DSPDFVIEWER_VERSION
#warning DSPDFVIEWER_VERSION was not set by the build system!
#define DSPDFVIEWER_VERSION "UNKNOWN"
#endif

#ifndef I3WORKAROUND_SHELLCODE
/* This will get executed once both windows are created, provided
 * the user passed --i3-workaround=true via command line or configuration file.
 *
 * You can override the shellcode by providing -DI3WORKAROUND_SHELLCODE="your shellcode"
 * at the cmake step.
 */
#define I3WORKAROUND_SHELLCODE "i3-msg '[class=\"dspdfviewer\" window_role=\"Audience_Window\"] move to output right, fullscreen' && " \
	" i3-msg '[class=\"dspdfviewer\" window_role=\"Secondary_Window\"]' fullscreen"
#endif

using namespace std;
using namespace boost::program_options;

RuntimeConfiguration::RuntimeConfiguration(int argc, const char* const * argv):
	m_useFullPage(false),
	m_showPresenterArea(true),
        m_duplicate(false),
	m_showWallClock(true),
	m_showThumbnails(true),
	m_thumbnailPagePart(PagePart::FullPage),
	m_showPresentationClock(true),
	m_showSlideClock(true),
	m_filePath(),
	m_hyperlinkSupport(true),
	m_cacheToMemory(true),
	m_cacheSizeMegaBytes(0),
	m_useSecondScreen(true),
	m_i3workaround(false),
	m_prerenderPreviousPages(3),
	m_prerenderNextPages(10),
	m_bottomPaneHeightPercent(20)
{
  options_description generic( tr("Generic options").toLocal8Bit().constData() );

/** FIXME: Using .toLocal8Bit() [.constData()] is Very Very Very Ugly,
 * but works because program_options copies the string...
 */
  generic.add_options()
    ("help,h", tr("Print help message").toLocal8Bit() )
    ("version,v", tr("Print version statement").toLocal8Bit() )
    ;

  options_description global( tr("Options affecting program behaviour")
	.toLocal8Bit().constData() );
  global.add_options()
    ("full-page,f", //value<bool>(&m_useFullPage)->default_value(false)->implicit_value(true),
      tr("Display the full slide on both screens (useful for PDFs created by presentation software other than latex-beamer)").toLocal8Bit() )
    ("prerender-previous-pages",
     value<unsigned>(&m_prerenderPreviousPages)->default_value(3),
     tr("Pre-render the preceding arg slides\n"
     "NOTE: If you set this to zero, you might not get a thumbnail for the previous slide unless it was loaded already."
	 ).toLocal8Bit()
    )
    ("prerender-next-pages",
     value<unsigned>(&m_prerenderNextPages)->default_value(10),
	 tr(
     "Pre-render the next arg slides\n"
     "NOTE: If you set this to zero, you might not get a thumbnail for the next slide unless it was loaded already."
	 ).toLocal8Bit()
     )
    ("hyperlink-support,l",
     value<bool>(&m_hyperlinkSupport)->default_value(true),
	 tr(
     "Support PDF Hyperlinks\n"
     "Follow hyperlinks when clicked (mouse pointer will change to a pointing hand) - set this to false if "
     "you cannot reliably control your mouse pointer position and want to always go ahead one slide on click.").toLocal8Bit()
	)
    ("cache-to-memory",
     value<bool>(&m_cacheToMemory)->default_value(true),
	 tr(
     "Cache the PDF file into memory\n"
     "Useful if you are editing the PDF file with latex while using the presenter software.").toLocal8Bit()
     )
	("cache-size",
		value<unsigned>(&m_cacheSizeMegaBytes)->default_value(1024),
		tr(
			"Size of the cache for pre-rendered pages, in megabytes."
		).toLocal8Bit()
	)
    ("i3-workaround",
     value<bool>(&m_i3workaround)->default_value(false),
	 tr(
     "Use i3 specific workaround: Execute shellcode once both windows have been created.")
#ifndef NDEBUG
	 .append( QString::fromUtf8(
     "\nDebug info: Shellcode is \n"
     I3WORKAROUND_SHELLCODE
	 ) )
#endif
	 .toLocal8Bit()
    )
    ;
  options_description secondscreen( tr("Options affecting the second screen").toLocal8Bit().constData() );
  secondscreen.add_options()
    ("use-second-screen,u",
     value<bool>(&m_useSecondScreen)->default_value(true),
	 tr(
     "Use the second screen. If you only have one monitor and just want to use this application as a fast, pre-caching PDF viewer"
     " you might want to say 0 here.\n"
     "NOTE: Whatever you say on -a, -t, -w, -s or -p doesn't matter if you set this to false.\n"
     "NOTE: You might want to say -f if you set this to false."
	 ).toLocal8Bit()
    )
    ("presenter-area,a",
     value<bool>(&m_showPresenterArea)->default_value(true),
	 tr(
     "Shows or hides the complete \"presenter area\" on the second screen, giving you a full-screen note page.\n"
     "NOTE: Whatever you say on -t, -w, -s or -p doesnt matter if you set this to false."
	 ).toLocal8Bit()
    )
    ("duplicate,d",
     value<bool>(&m_duplicate)->default_value(false),
     tr("Duplicates the audience's screen next to the notes on the second screen.").toLocal8Bit()
    )
    ("thumbnails,t",
     value<bool>(&m_showThumbnails)->default_value(true),
	 tr(
     "Show thumbnails of previous, current and next slide").toLocal8Bit()
	)
	("thumbnail-page-part,T",
		value<PagePart>(&m_thumbnailPagePart)->default_value(PagePart::FullPage),
		tr("Thumbnails show this page part. Valid values are \"left\", \"right\" or \"both\"").toLocal8Bit()
	)
    ("wall-clock,w",
     value<bool>(&m_showWallClock)->default_value(true),
	 tr(
     "Show the wall clock").toLocal8Bit()
	)
    ("presentation-clock,p",
     value<bool>(&m_showPresentationClock)->default_value(true),
     tr(
		 "Show the presentation clock").toLocal8Bit()
	)
    ("slide-clock,s",
     value<bool>(&m_showSlideClock)->default_value(true),
     tr("Show the slide clock").toLocal8Bit()
	)
    ("bottom-pane-height,b",
     value<unsigned>(&m_bottomPaneHeightPercent)->default_value(20),
     tr("Percentage of second screen to use for the bottom pane").toLocal8Bit()
	)
    ;

  options_description hidden(tr("Hidden options").toLocal8Bit().constData());
  hidden.add_options()
    ("pdf-file", value< string >(&m_filePath), tr("PDF File to display").toLocal8Bit())
    ;
  positional_options_description p;
  p.add("pdf-file", 1);

  options_description help;
  help.add(generic).add(global).add(secondscreen);



  options_description commandLineOptions;
  commandLineOptions.add(help).add(hidden);

  options_description configFileOptions;
  configFileOptions.add(global).add(secondscreen);

  variables_map vm;
  store( command_line_parser(argc,argv).options(commandLineOptions).positional(p).run(), vm);
  QString configurationFileLocation = QString::fromUtf8( qgetenv("HOME").append("/.config/dspdfviewer.ini") );
  {
    // See if the configuration file exists and is readable
    std::ifstream cfile( qPrintable(configurationFileLocation) );
    if ( cfile.good() ) {
      store( parse_config_file( cfile, configFileOptions), vm);
    }
  } // close input file
  notify(vm);

  if ( vm.count("version") || vm.count("help") ) {
    cout << "dspdfviewer version " << DSPDFVIEWER_VERSION << endl;
	cout << tr( "Written by Danny Edel\n"
			"\n"
			"Copyright (C) 2012 Danny Edel.\n"
			"This is free software; see the source for copying conditions.  There is NO\n"
			"warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.").toLocal8Bit().constData() << endl;
    if ( vm.count("help")) {
      cout << endl;
      cout << tr("Usage: %1 [options] pdf-file").arg( QString::fromUtf8(argv[0]) ).toLocal8Bit().constData() << endl;
      cout << help << endl;
      /*: Please try to keep line length below 70 chars and use \t (tab) for padding */
		cout << tr("Interactive Controls:\n"
			"\tPress F1 or ? during program execution to get a quick\n"
			"\toverview about available controls.\n"
			"\tPlease read the manpage (man dspdfviewer) for the full list.").toLocal8Bit().constData() << endl;
    }
    exit(0);
  }

  if ( m_bottomPaneHeightPercent < 1 || m_bottomPaneHeightPercent > 99 ) {
    throw std::runtime_error( tr("Invalid height in specified. Please use a value from 1 to 99 (inclusive)").toLocal8Bit().constData() );
  }
  
  m_useFullPage = ( 0 < vm.count("full-page") );

  /** Implied options */
  if ( ! m_useSecondScreen ) {
    /* If we dont use a second screen, there's no point in using the presenter area */
    m_showPresenterArea = false;
  }
  if ( ! m_showPresenterArea ) {
    /* If the presenter area is hidden, disable all clocks and the thumbnails */
    m_showPresentationClock = false;
    m_showWallClock = false;
    m_showSlideClock = false;
    /* This option will effectively disable rendering of thumbnails */
    m_showThumbnails = false;
  }
}

string RuntimeConfiguration::filePath() const
{
  if ( m_filePath.empty() ) {
    throw noFileNameException();
  }

  return m_filePath;
}

QString RuntimeConfiguration::filePathQString() const
{
  return QString::fromLocal8Bit( filePath().c_str() );
}

bool RuntimeConfiguration::useFullPage() const
{
  return m_useFullPage;
}

bool RuntimeConfiguration::showPresentationClock() const
{
  return m_showPresentationClock;
}
bool RuntimeConfiguration::showPresenterArea() const
{
  return m_showPresenterArea;
}
bool RuntimeConfiguration::duplicate() const
{
  return m_duplicate;
}
bool RuntimeConfiguration::showSlideClock() const
{
  return m_showSlideClock;
}
bool RuntimeConfiguration::showThumbnails() const
{
  return m_showThumbnails;
}
bool RuntimeConfiguration::showWallClock() const
{
  return m_showWallClock;
}

unsigned int RuntimeConfiguration::prerenderNextPages() const
{
  return m_prerenderNextPages;
}
unsigned int RuntimeConfiguration::prerenderPreviousPages() const
{
  return m_prerenderPreviousPages;
}

bool RuntimeConfiguration::useSecondScreen() const
{
  return m_useSecondScreen;
}

PDFCacheOption RuntimeConfiguration::cacheSetting() const
{
  return m_cacheToMemory?
	PDFCacheOption::keepPDFinMemory :
	PDFCacheOption::rereadFromDisk;
}

unsigned int RuntimeConfiguration::bottomPaneHeight() const
{
  return m_bottomPaneHeightPercent;
}

bool RuntimeConfiguration::hyperlinkSupport() const
{
  return m_hyperlinkSupport;
}

void RuntimeConfiguration::filePath(const std::string& newPath )
{
	m_filePath = newPath;
}

bool RuntimeConfiguration::filePathDefined() const
{
	return ! m_filePath.empty();
}

noFileNameException::noFileNameException():
	logic_error( QCoreApplication::translate("DSPDFViewer", "You did not specify a PDF-File to display.").toLocal8Bit().constData() ) {
}

bool RuntimeConfiguration::i3workaround() const
{
	return m_i3workaround;
}

std::string RuntimeConfiguration::i3workaround_shellcode() const
{
	return std::string( I3WORKAROUND_SHELLCODE );
}

PagePart RuntimeConfiguration::thumbnailPagePart() const
{
	return m_thumbnailPagePart;
}

unsigned RuntimeConfiguration::cacheSizeBytes() const {
	return cacheSizeMegaBytes()*1024*1024;
}

unsigned RuntimeConfiguration::cacheSizeMegaBytes() const{
	return m_cacheSizeMegaBytes;
}
