#ifndef DEFAULTFILECONTAINER_H_
#define DEFAULTFILECONTAINER_H_

#include "../default_ns.h"
#include "../../../filesystem/interfaces/filesystemifilecontainer.h"


DEFAULT_PLUGIN_NS_BEGIN

class FileContainer : public IFileContainer, public IFileContainerScanner
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

private:
	void scan(InfoListItem *root, const volatile BaseTask::Flags &aborted) const;

private:
	friend class CopyControl;
	QString m_path;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTFILECONTAINER_H_ */
