#ifndef LIBARCHIVEFILEINFO_H_
#define LIBARCHIVEFILEINFO_H_

#include "../libarchive_ns.h"
#include "../../../../filesystem/interfaces/filesystemifileinfo.h"


LIBARCHIVE_ARC_PLUGIN_NS_BEGIN

class Info : public IFileInfo, public IFileType
{
public:
	struct Data
	{
		QByteArray path;
		QString fileName;
		size_type fileSize;
		QDateTime lastModified;
	};

public:
	Info();
	Info(const Data &data, bool isDir);

	/* IFileInfo */
	virtual bool isDir() const;
	virtual bool isFile() const;
	virtual bool isLink() const;
	virtual size_type fileSize() const;
	virtual QString fileName() const;
	virtual const IFileType *fileType() const;
	virtual QDateTime lastModified() const;
	virtual int permissions() const;

	/* IFileType */
	virtual FileTypeId id() const;
	virtual QIcon icon() const;
	virtual QString name() const;
	virtual QString description() const;

	const QByteArray &path() const { return m_data.path; }

private:
	Data m_data;
	bool m_isDir;
	FileTypeInfo m_fileTypeInfo;
};

LIBARCHIVE_ARC_PLUGIN_NS_END

#endif /* LIBARCHIVEFILEINFO_H_ */
