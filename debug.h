#pragma once

#include <QDebug>

#ifdef QT_NO_DEBUG_OUTPUT

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

#if defined(_MSC_VER)
#define __func__ __FUNCTION__
#endif

#define DEBUGOUT qDebug() << \
 QString::fromAscii("%1:%2 [%3()]"). \
 arg(QFileInfo( QString::fromAscii(__FILE__) ).fileName()). \
 arg(QString::number( __LINE__ )). \
 arg(QString::fromAscii(__func__) )

#endif
