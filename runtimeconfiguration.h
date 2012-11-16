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
  
  /** complete path to the PDF file */
  std::string m_filePath;
public:
  
  /** fill the variables based on the C-style arguments to main()
   * 
   * Note: Might call exit() if a terminating option like --help
   * or --version was called.
   * 
   * Note: Might throw exceptions if not parsable.
   */
  void parse(int argc, char** argv);
  
  bool useFullPage() const;
  
  QString filePathQString() const;
  
  std::string filePath() const;
};

#endif // RUNTIMECONFIGURATION_H
