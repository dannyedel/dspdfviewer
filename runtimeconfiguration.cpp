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

#ifndef DSPDFVIEWER_VERSION
#warning DSPDFVIEWER_VERSION was not set by the build system!
#define DSPDFVIEWER_VERSION "UNKNOWN"
#endif

using namespace std;
using namespace boost::program_options;

RuntimeConfiguration::RuntimeConfiguration(int argc, char** argv)
{
  options_description generic("Generic options");
  
  generic.add_options()
    ("help,h", "Print help message")
    ("version,v", "Print version statement")
    ;
  
  options_description global("Options affecting program behaviour");
  global.add_options()
    ("full-page,f", //value<bool>(&m_useFullPage)->default_value(false)->implicit_value(true),
      "Display the full slide on both screens (useful for PDFs created by presentation software other than latex-beamer)")
    ("prerender-previous-pages",
     value<unsigned>(&m_prerenderPreviousPages)->default_value(3),
     "Pre-render the preceding arg slides\n"
     "NOTE: If you set this to zero, you might not get a thumbnail for the previous slide unless it was loaded already."
    )
    ("prerender-next-pages",
     value<unsigned>(&m_prerenderNextPages)->default_value(10),
     "Pre-render the next arg slides\n"
     "NOTE: If you set this to zero, you might not get a thumbnail for the next slide unless it was loaded already."
     )
    ("hyperlink-support,l",
     value<bool>(&m_hyperlinkSupport)->default_value(false),
     "Support PDF Hyperlinks\n"
     "NOTE: If you set this to yes, watch where you put your mouse cursor / presenter")
    ;
  options_description secondscreen("Options affecting the second screen");
  secondscreen.add_options()
    ("use-second-screen,u",
     value<bool>(&m_useSecondScreen)->default_value(true),
     "Use the second screen. If you only have one monitor and just want to use this application as a fast, pre-caching PDF viewer"
     " you might want to say 0 here.\n"
     "NOTE: Whatever you say on -a, -t, -w, -s or -p doesn't matter if you set this to false.\n"
     "NOTE: You might want to say -f if you set this to false."
    )
    ("presenter-area,a",
     value<bool>(&m_showPresenterArea)->default_value(true),
     "Shows or hides the complete \"presenter area\" on the second screen, giving you a full-screen note page.\n"
     "NOTE: Whatever you say on -t, -w, -s or -p doesnt matter if you set this to false."
    )
    ("thumbnails,t",
     value<bool>(&m_showThumbnails)->default_value(true),
     "Show thumbnails of previous, current and next slide")
    ("wall-clock,w",
     value<bool>(&m_showWallClock)->default_value(true),
     "Show the wall clock")
    ("presentation-clock,p",
     value<bool>(&m_showPresentationClock)->default_value(true),
     "Show the presentation clock")
    ("slide-clock,s",
     value<bool>(&m_showSlideClock)->default_value(true),
     "Show the slide clock")
    ;
  
  options_description hidden("Hidden options");
  hidden.add_options()
    ("pdf-file", value< string >(&m_filePath), "PDF File to display")
    ;
  positional_options_description p;
  p.add("pdf-file", 1);
   
  options_description help;
  help.add(generic).add(global).add(secondscreen);
  
  
  
  options_description commandLineOptions;
  commandLineOptions.add(help).add(hidden);
  
  options_description configFileOptions;
  configFileOptions.add(global).add(secondscreen);
  
  /// TODO Parse config file
  
  variables_map vm;
  store( command_line_parser(argc,argv).options(commandLineOptions).positional(p).run(), vm);
  notify(vm);
  
  if ( vm.count("version") || vm.count("help") ) {
    cout << "dspdfviewer version " << DSPDFVIEWER_VERSION << endl;
    cout << "Written by Danny Edel" << endl;
    cout << endl;
    cout << "Copyright (C) 2012 Danny Edel." << endl
	 << "This is free software; see the source for copying conditions.  There is NO" << endl
	 << "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE." << endl;
    if ( vm.count("help")) {
      cout << endl;
      cout << "Usage: " << argv[0] << " [options] pdf-file" << endl;
      cout << help << endl;
    }
    exit(1);
  }
  
  if ( 0 == vm.count("pdf-file") ) {
    throw std::runtime_error("You did not specify a PDF-File to display");
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
  return m_filePath;
}

QString RuntimeConfiguration::filePathQString() const
{
  return QString::fromStdString(m_filePath);
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

