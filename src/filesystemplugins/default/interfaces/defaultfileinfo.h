#ifndef DEFAULTFILEINFO_H_
#define DEFAULTFILEINFO_H_

#include "../default_ns.h"
#include "../../../filesystem/filetypeinfo/filetypeinfo.h"
#include "../../../filesystem/interfaces/filesystemifileinfo.h"


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

    template <int I>
    explicit Info(const QString &filePath, int_to_type<I>);

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
	bool isRoot() const { return m_isRoot; }

private:
	void refresh();

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
    QString m_filePath;
    QString m_fileName;
    FileInfo m_info;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTFILEINFO_H_ */
