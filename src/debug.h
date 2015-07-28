#pragma once

#include <QDebug>

#if QT_NO_DEBUG_OUTPUT

/* Qt is defined to squelch debug output. Simply forward to
 * qDebug()'s definition.
 */
#define DEBUGOUT qDebug()

#else

#include <QtGlobal>
#include <QFileInfo>

/* We're debugging.
 * Add crazy mumbo jumbo here.
 */


/** Print filename / line / function info
 */

#define DEBUGOUT qDebug() << \
 QString("%1:%2 [%3()]"). \
 arg(QFileInfo(__FILE__).fileName()). \
 arg(QString::number( __LINE__ )). \
 arg(__func__)

#endif
