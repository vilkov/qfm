#include "filesystemfoldernodefile.h"


FILE_SYSTEM_NS_BEGIN

File::File(const QString &absoluteFilePath) :
	m_file(absoluteFilePath)
{}

QString File::lastError() const
{
	return m_file.errorString();
}

File::size_type File::size() const
{
	return m_file.size();
}

bool File::seek(size_type off)
{
	return m_file.seek(off);
}

File::size_type File::read(uchar *data, size_type size)
{
	return m_file.read((char*)data, size);
}

File::size_type File::write(const uchar *data, size_type size)
{
	return m_file.write((const char*)data, size);
}

bool File::open(QFile::OpenMode mode)
{
	return m_file.open(mode);
}

FILE_SYSTEM_NS_END
