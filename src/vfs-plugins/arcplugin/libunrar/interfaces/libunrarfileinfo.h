#ifndef LIBUNRARFILEINFO_H_
#define LIBUNRARFILEINFO_H_

#include <vfs/interfaces/vfs_ifileinfo.h>
#include "../libunrar_ns.h"


LIBUNRAR_ARC_PLUGIN_NS_BEGIN

class Info : public IFileInfo, public IFileType
{
public:
	struct Data
	{
		QString path;
		Location fileName;
		size_type fileSize;
		QDateTime lastModified;
	};

public:
	Info(const Data &data, bool isDir);

	/* IFileInfo */
	virtual bool isDir() const;
	virtual bool isFile() const;
	virtual bool isLink() const;
	virtual size_type fileSize() const;
	virtual const Location &fileName() const;
	virtual const IFileType *fileType() const;
	virtual QDateTime lastModified() const;
	virtual int permissions() const;

	/* IFileType */
	virtual FileTypeId id() const;
	virtual QIcon icon() const;
	virtual QString name() const;
	virtual QString description() const;

	const QString &path() const { return m_data.path; }

	static Location location(const QString &label);

private:
	Data m_data;
	bool m_isDir;
	FileTypeInfo m_fileTypeInfo;
};

LIBUNRAR_ARC_PLUGIN_NS_END

#endif /* LIBUNRARFILEINFO_H_ */
