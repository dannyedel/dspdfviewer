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


#ifndef HYPERLINKAREA_H
#define HYPERLINKAREA_H

#include <QLabel>
#include <poppler/qt4/poppler-qt4.h>

#include "adjustedlink.h"

class HyperlinkArea : public QLabel
{
  Q_OBJECT
  
  /** FIXME Exception class */
  struct WrongLinkType{};
  
  uint targetPage;
public:
  HyperlinkArea(QLabel* imageLabel, const AdjustedLink& gotoLink);
  
  virtual void mousePressEvent(QMouseEvent* ev);
  
  signals:
    void gotoPageRequested(uint targetPage);
    
    
};

#endif // HYPERLINKAREA_H
