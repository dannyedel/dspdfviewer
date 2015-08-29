#include "testhelpers.hh"

#include <QDir>

QString TestHelpers::pdfFilename() {
	static const QString relativePart = "colored-rectangles.pdf";
	static const QDir myDir = QDir::current();
	if ( ! myDir.exists(relativePart) ) {
		throw std::runtime_error("Could not find the test pdf. This is bad.");
	}
	return myDir.filePath(relativePart);
}
