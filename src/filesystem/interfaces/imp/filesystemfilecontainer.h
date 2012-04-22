#ifndef FILESYSTEMFILECONTAINER_H_
#define FILESYSTEMFILECONTAINER_H_

#include "../filesystemifilecontainer.h"


FILE_SYSTEM_NS_BEGIN

class FileContainer : public IFileContainer
{
public:
	FileContainer(const QString &path);

	/* IFileLocation */
	virtual QString location() const;
	virtual QString location(const QString &fileName) const;

	/* IFileContainer */
	virtual bool isPhysical() const;
	virtual IFileInfo::size_type freeSpace() const;
	virtual ICopyControl *createControl(INodeView *view) const;

	virtual bool contains(const QString &fileName) const;
	virtual bool remove(const QString &fileName, QString &error) const;
	virtual bool rename(const QString &oldName, const QString &newName, QString &error) const;

	virtual IFileContainer *open() const;
	virtual IFileAccessor *open(const QString &fileName, int mode, QString &error) const;
	virtual IFileContainer *open(const QString &fileName, bool create, QString &error) const;

protected:
	friend class CopyControl;
	QString m_path;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFILECONTAINER_H_ */
