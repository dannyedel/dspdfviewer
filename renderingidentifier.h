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


#ifndef RENDERINGIDENTIFIER_H
#define RENDERINGIDENTIFIER_H
#include "pagepart.h"
#include <qglobal.h>
#include <qsize.h>
#include <QString>

class RenderingIdentifier
{
private:
  int thePageNumber;
  PagePart thePagePart;
  QSize theRequestedPageSize;
  
public:
  RenderingIdentifier(int pagenum, PagePart pagepart, QSize requestedPageSize);
  int pageNumber() const;
  PagePart pagePart() const;
  QSize requestedPageSize() const;
  
  bool operator == (const RenderingIdentifier& other) const;
  
  /** Cast to a string that is usable as a hash identifier **/
  operator QString() const;
};

#endif // RENDERINGIDENTIFIER_H
