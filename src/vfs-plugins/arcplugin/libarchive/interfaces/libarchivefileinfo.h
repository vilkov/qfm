#ifndef LIBARCHIVEFILEINFO_H_
#define LIBARCHIVEFILEINFO_H_

#include <vfs/interfaces/vfs_ifileinfo.h>
#include "../libarchive_ns.h"


LIBARCHIVE_ARC_PLUGIN_NS_BEGIN

class Info : public IFileInfo, public IFileType
{
public:
	struct Data
	{
		QByteArray path;
		Location fileName;
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
	virtual const Location &fileName() const;
	virtual const IFileType *fileType() const;
	virtual const QDateTime &lastModified() const;
	virtual int permissions() const;

	/* IFileType */
	virtual const FileTypeId &id() const;
	virtual const QIcon &icon() const;
	virtual const QString &name() const;
	virtual const QString &description() const;

	const QByteArray &path() const { return m_data.path; }

	static Location location(const QByteArray &location);

private:
	Data m_data;
	bool m_isDir;
	FileTypeInfo m_fileTypeInfo;
};

LIBARCHIVE_ARC_PLUGIN_NS_END

#endif /* LIBARCHIVEFILEINFO_H_ */
