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

TestHelpers::RetCode::RetCode(const bool& b):
	b_(b) {
}

TestHelpers::RetCode::operator int() {
	if ( b_ == true )
		return 0;
	return 1;
}

TestHelpers::RetCode::operator bool() {
	return b_;
}
