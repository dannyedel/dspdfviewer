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


#include "renderingidentifier.h"

bool RenderingIdentifier::operator==(const RenderingIdentifier& other) const
{
  return
    thePageNumber == other.thePageNumber
    && thePagePart == other.thePagePart
    && theRequestedPageSize == other.theRequestedPageSize;
}

RenderingIdentifier::operator QString() const
{
  QString s("page%1_%2_size%3x%4");
  QString partId;
  switch( pagePart() ) {
    case PagePart::LeftHalf:
      partId="LeftHalf";
      break;
    case PagePart::RightHalf:
      partId="RightHalf";
      break;
    case PagePart::FullPage:
	partId="FullPage";
	break;
  }
  return s.arg(pageNumber()).arg(partId).arg(requestedPageSize().width()).arg(requestedPageSize().height());
}

int RenderingIdentifier::pageNumber() const
{
  return thePageNumber;
}

PagePart RenderingIdentifier::pagePart() const
{
  return thePagePart;
}

QSize RenderingIdentifier::requestedPageSize() const
{
  return theRequestedPageSize;
}

RenderingIdentifier::RenderingIdentifier(int pagenum, PagePart pagepart, QSize pagesize):
  thePageNumber(pagenum), thePagePart(pagepart), theRequestedPageSize(pagesize)
{
  
}



