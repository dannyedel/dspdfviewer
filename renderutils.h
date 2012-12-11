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


#ifndef RENDERUTILS_H
#define RENDERUTILS_H

#include <QSharedPointer>
#include "pagepart.h"
#include <QImage>
#include <poppler/qt4/poppler-qt4.h>

class RenderUtils
{
public:
  static QImage renderPagePart(QSharedPointer<Poppler::Page> page, QSize targetSize, PagePart whichPart);
  
  /** Since only the static functions of this class are used, we do not need to construct instances */
  RenderUtils() =delete;
};

#endif // RENDERUTILS_H
