#include "testhelpers.hh"

#include <QDir>

QString TestHelpers::pdfFilename() {
	const QString relativePart = "colored-rectangles.pdf";
	const QDir myDir = QDir::current();
	if ( ! myDir.exists(relativePart) ) {
		throw std::runtime_error("Could not find the test pdf. This is bad.");
	}
	return myDir.filePath(relativePart);
}
