#include "pdfdocumentreference.h"
#include "pdfpagereference.h"

#include <stdexcept>
#include <QFile>
#include <QDebug>

QSharedPointer< Poppler::Document > PDFDocumentReference::popplerDocument() const
{
  QSharedPointer<Poppler::Document> m_document;
  
  if ( cacheOption() == PDFCacheOption::rereadFromDisk ) {
    qDebug() << "Trying to build a Poppler::Document from file" << filename();
    QSharedPointer<Poppler::Document> diskDocument( Poppler::Document::load(filename()) );
    m_document.swap(diskDocument);
  }
  else if ( cacheOption() == PDFCacheOption::keepPDFinMemory ) {
    qDebug() << "Trying to build a document from" << fileContents_.size() << "byte memory cache";
    QSharedPointer<Poppler::Document> memoryDocument( Poppler::Document::loadFromData(fileContents_) );
    m_document.swap(memoryDocument);
  }
  if ( !m_document || m_document->isLocked() )
    throw std::runtime_error("Document not readable");
  m_document->setRenderHint(Poppler::Document::Antialiasing, true);
  m_document->setRenderHint(Poppler::Document::TextAntialiasing, true);
  m_document->setRenderHint(Poppler::Document::TextHinting, true);
  return m_document;
}

PDFPageReference PDFDocumentReference::page(unsigned int pageNumber) const
{
  PDFPageReference ppr(*this, pageNumber);
  return ppr;
}

PDFDocumentReference::PDFDocumentReference(const QString& theFilename, const PDFCacheOption& theCacheOption):
filename_(theFilename),
cacheOption_(theCacheOption)
{
  if ( cacheOption() == PDFCacheOption::keepPDFinMemory ) {
    qDebug() << "Reading file into memory";
    QFile file(filename());
    file.open(QIODevice::ReadOnly);
    fileContents_ = file.readAll();
    qDebug() << fileContents_.size() << "bytes read";
  }
}
const PDFCacheOption& PDFDocumentReference::cacheOption() const
{
  return cacheOption_;
}

const QString& PDFDocumentReference::filename() const
{
  return filename_;
}

PDFDocumentReference& PDFDocumentReference::operator=(const PDFDocumentReference& rhs)
{
  if ( rhs.filename() != filename() ) {
    throw std::runtime_error("This PDFDocumentReference has a different filename");
  }
  if ( rhs.cacheOption() != cacheOption() ) {
    throw std::runtime_error("This PDFDocumentReference has a different cache setting");
  }
  fileContents_=rhs.fileContents_;
  return *this;
}

bool operator==(const PDFDocumentReference& lhs, const PDFDocumentReference& rhs)
{
  if ( lhs.cacheOption() != rhs.cacheOption() ) {
    return false;
  }
  else if ( lhs.cacheOption() == PDFCacheOption::keepPDFinMemory ) {
    return lhs.fileContents_ == rhs.fileContents_;
  }
  else {
    return lhs.filename() == rhs.filename();
  }
}

