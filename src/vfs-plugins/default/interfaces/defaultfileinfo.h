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

public:
	static QTextCodec *codec();
	static FileTypeInfo fileTypeInfoFromFileName(const QByteArray &fileName, bool isDir);

#ifdef Q_OS_UNIX
	static QIcon fileTypeIcon(const char *mimeType, int size);
#endif

	void refresh();
	void identify();
	bool isRoot() const { return m_isRoot; }
	bool exists() const { return m_info.exists; }

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
    Location m_fileName;
    FileInfo m_info;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTFILEINFO_H_ */
