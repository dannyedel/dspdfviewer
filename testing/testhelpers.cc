#include "testhelpers.hh"

#include <QDir>

QString TestHelpers::pdfFilename(const std::string& basename) {
	const QString relativePart = QString::fromLocal8Bit( basename.c_str() );
	const QDir myDir = QDir::current();
	if ( ! myDir.exists(relativePart) ) {
		throw std::runtime_error("Could not find the test pdf. This is bad.");
	}
	return myDir.filePath(relativePart);
}

std::ostream& operator << (std::ostream& where, const QSize& what) {
	return where << "QSize(" << what.width() << 'x' << what.height() << ')';
}

std::ostream& operator << (std::ostream& where, const QColor& what) {
	return where << "QColor(" << qPrintable( what.name() ) << ")";
}
