#ifndef DEFAULTFILEINFO_H_
#define DEFAULTFILEINFO_H_

#include <QtCore/QTextCodec>
#include <vfs/filetypeinfo/vfs_filetypeinfo.h>
#include <vfs/interfaces/vfs_ifileinfo.h>
#include "../default_ns.h"


DEFAULT_PLUGIN_NS_BEGIN

class Info : public IFileInfo, public IFileType
{
public:
	template <int I> struct int_to_type {enum { value = I };};
	typedef int_to_type<0> None;
	typedef int_to_type<1> Refresh;
	typedef int_to_type<2> Identify;

public:
    explicit Info();
    explicit Info(const Info &other);

    template <typename T>
    explicit Info(const QByteArray &filePath, T = T());

    template <typename T>
    explicit Info(const Info &other, T = T());

    QString operator-(const Info &other) const;

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

public:
	static QTextCodec *codec();

	void refresh();
	bool isRoot() const { return m_isRoot; }
	bool exists() const { return m_info.exists; }
    const QByteArray &rawFileName() const { return m_fileName; }

private:
	struct FileInfo
	{
		FileInfo() :
			isDir(false),
			isFile(false),
			isLink(false),
			exists(false),
			permissions(0),
			size(0)
		{}

	    bool isDir;
	    bool isFile;
	    bool isLink;
	    bool exists;
	    int permissions;
	    qint64 size;
	    QDateTime lastModified;
	    FileTypeInfo type;
	};

private:
    bool m_isRoot;
    QByteArray m_filePath;
    QByteArray m_fileName;
    QString m_fileNameString;
    FileInfo m_info;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTFILEINFO_H_ */
