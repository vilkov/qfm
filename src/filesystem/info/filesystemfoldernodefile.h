#ifndef FILESYSTEMFOLDERNODEFILE_H_
#define FILESYSTEMFOLDERNODEFILE_H_

#include <QtCore/QFile>
#include "../interfaces/filesystemifile.h"


FILE_SYSTEM_NS_BEGIN

class File : public IFile
{
public:
	File(const QString &absoluteFilePath);

	/* IFile */
	virtual QString lastError() const;
	virtual size_type size() const;

	virtual bool seek(size_type off);
	virtual size_type read(value_type *data, size_type size);
	virtual size_type write(const value_type *data, size_type size);

public:
	bool open(QFile::OpenMode mode);

private:
	QFile m_file;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERNODEFILE_H_ */
