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


#ifndef RUNTIMECONFIGURATION_H
#define RUNTIMECONFIGURATION_H
#include <string>
#include <QString>

class RuntimeConfiguration
{
  /** Use the program to render a standard PDF (i.e. display
   * the full page on both sides)
   */
  bool	m_useFullPage;
  
  /** Show presenter area
   */
  bool m_showPresenterArea;
  
  /** Show the wall clock
   */
  bool m_showWallClock;
  
  /** Show the thumbnails of previous, this and next slide */
  bool m_showThumbnails;
  
  /** Show the total presentation time **/
  bool m_showPresentationClock;
  
  /** Show the current slide time **/
  bool m_showSlideClock;
  
  /** complete path to the PDF file */
  std::string m_filePath;
  
  /** Support PDF Hyperlinks**/
  bool m_hyperlinkSupport;

  /** Shall the complete PDF be read into memory */
  bool m_cacheToMemory;
  
  /** Single-Display mode
   * 
   * If True, there is only the audience display, the presenter's screen will remain hidden
   * Probably most useful with -f
   */
  bool m_useSecondScreen;
  
  /** Make sure that so many previous pages are pre-rendered
   * (Probably wont make sense until you can jump to slide
   * n without visiting 0..(n-1) first, but once PDF hyperlinks
   * are enabled, this will be quite useful.
   */
  unsigned m_prerenderPreviousPages;
  
  /** Make sure so many next pages are pre-rendered
   */
  unsigned m_prerenderNextPages;
public:
  
  /** fill the variables based on the config file and the C-style arguments to main()
   * 
   * Note: Reads the config file before command-line arguments.
   * Command-line overrides config.
   * 
   * Note: Might call exit() if a terminating option like --help
   * or --version was called.
   * 
   * Note: Might throw exceptions if not parsable.
   */
  RuntimeConfiguration(int argc, char** argv);
  
  bool useFullPage() const;
  
  QString filePathQString() const;
  
  std::string filePath() const;
  
  bool showPresenterArea() const;
  bool showWallClock() const;
  bool showThumbnails() const;
  bool showPresentationClock() const;
  bool showSlideClock() const;
  
  unsigned prerenderPreviousPages() const;
  unsigned prerenderNextPages() const;
  
  bool useSecondScreen() const;
  
  bool cachePDFToMemory() const;
};

#endif // RUNTIMECONFIGURATION_H
