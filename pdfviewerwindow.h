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


#ifndef PDFVIEWERWINDOW_H
#define PDFVIEWERWINDOW_H

#include <qwidget.h>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMap>

class DSPDFViewer;
/** Shared base class for both windows (primary and secondary)
 * 
 */
class PDFViewerWindow : public QWidget
{
  Q_OBJECT
private:
  unsigned int m_monitor;
  DSPDFViewer* m_dspdfviewer;
  QLabel imageLabel;
  bool informationLineVisible;
  QVBoxLayout* outerlayout;
  QHBoxLayout* innerlayout;
  QHBoxLayout* informationLineLayout;
  QMap<int, QImage> thumbnails;
  QMap<int, QLabel*> thumbnailLabels;

public:
  /** Standard constructor
   * @param monitor monitor to start on (usually 0 for primary)
   * @param dspdfviewer Pointer to the application (to handle next/prev commands)
   */
    explicit PDFViewerWindow(unsigned int monitor);
    
    /** Sets the monitor to display this window on
     * Automatically calls reposition
     */
    void setMonitor(const unsigned int monitor);
    
    /** Gets the current monitor setting
     */
    unsigned int getMonitor() const;
    
    /** Reposition the window (for example after a monitor change)
     */
    void reposition();
    
    /** Display this image
     */
    void displayImage(QImage image);
    
    virtual void wheelEvent(QWheelEvent* e);
    
    virtual void keyPressEvent(QKeyEvent* e);
    
    virtual void mousePressEvent(QMouseEvent* e);
    
    void setViewer(DSPDFViewer* v);
    
    QSize getTargetImageSize() const;
    
        
    void showInformationLine();
    
    void hideInformationLine();
    
    bool isInformationLineVisible() const;
    
    void addThumbnail(int pageNumber, QImage thumbnail);
    
    void renderThumbnails(int currentPage);
    
    bool hasThumbnailForPage(int pageNumber) const;
};

#endif // PDFVIEWERWINDOW_H
