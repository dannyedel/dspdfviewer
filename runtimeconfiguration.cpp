/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2012  <copyright holder> <email>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "runtimeconfiguration.h"

#include <boost/program_options.hpp>
#include <iostream>

using namespace std;
using namespace boost::program_options;

void RuntimeConfiguration::parse(int argc, char** argv)
{
  options_description generic("Generic options");
  
  generic.add_options()
    ("help,h", "Print help message")
    ;
  
  options_description global("Options affecting program behaviour");
  global.add_options()
    ("full-page,f", //value<bool>(&m_useFullPage)->default_value(false)->implicit_value(true),
      "Display the full slide on both screens (useful for PDFs created by presentation software other than latex-beamer)")
    ;
  options_description secondscreen("Options affecting the second screen");
  secondscreen.add_options()
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
  
  if ( vm.count("help")) {
    cout << "Usage: " << argv[0] << " [options] pdf-file" << endl;
    cout << help << endl;
    exit(1);
  }
  
  if ( 0 == vm.count("pdf-file") ) {
    throw std::runtime_error("You did not specify a PDF-File to display");
  }
  
  m_useFullPage = ( 0 < vm.count("full-page") );
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

