#ifndef FILESYSTEMFILEINFO_H_
#define FILESYSTEMFILEINFO_H_

#include "../filesystemifileinfo.h"
#include "../../filetypeinfo/filetypeinfo.h"


FILE_SYSTEM_NS_BEGIN

class Info : public IFileInfo
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

	/* IFileType */
	virtual FileTypeId id() const;
	virtual QIcon icon() const;
	virtual QString name() const;
	virtual QString description() const;

	/* IFileInfo */
	virtual bool isDir() const;
	virtual bool isFile() const;
	virtual bool isLink() const;
	virtual size_type fileSize() const;
	virtual QString fileName() const;
	virtual QDateTime lastModified() const;
	virtual int permissions() const;

public:
	bool isRoot() const { return m_isRoot; }
	bool shouldBeUpdatedBy(const Info &other) const
	{
		return
			m_info.lastModified != other.m_info.lastModified ||
			m_info.size != other.m_info.size ||
			m_info.type.name.isEmpty();
	}

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

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFILEINFO_H_ */
