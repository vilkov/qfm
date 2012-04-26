#ifndef FILESYSTEMROOTFILECONTAINER_H_
#define FILESYSTEMROOTFILECONTAINER_H_

#include "../interfaces/filesystemifilecontainer.h"


FILE_SYSTEM_NS_BEGIN

class RootFileContainer : public IFileContainer, public IFileContainerScanner, public IFileInfo, public IFileType
{
public:
	virtual RootFileContainer();

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
	virtual bool move(const IFileContainer *source, const QString &fileName, QString &error) const;

	virtual IFileContainer *open() const;
	virtual IFileAccessor *open(const QString &fileName, int mode, QString &error) const;
	virtual IFileContainer *open(const QString &fileName, bool create, QString &error) const;

	virtual const IFileContainerScanner *scanner() const;

	/* IFileContainerScanner */
	virtual void enumerate(IEnumerator::Holder &enumerator) const;
	virtual IFileInfo *info(const QString &fileName, QString &error) const;
	virtual void scan(Snapshot &snapshot, const volatile BaseTask::Flags &aborted) const;
	virtual void refresh(Snapshot &snapshot, const volatile BaseTask::Flags &aborted) const;

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

private:
	QString m_path;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMROOTFILECONTAINER_H_ */
