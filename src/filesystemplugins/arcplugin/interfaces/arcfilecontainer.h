#ifndef ARCFILECONTAINER_H_
#define ARCFILECONTAINER_H_

#include "../arcplugin_ns.h"
#include "../../../filesystem/interfaces/filesystemifilecontainer.h"
#include "../../../filesystem/containers/filesystemwrappednodeitem.h"


ARC_PLUGIN_NS_BEGIN

class FileContainer : public IFileContainer, public IFileContainerScanner
{
public:
	static IFileContainer *create(const IFileContainer *container, const IFileInfo *file, QString &error);

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
	virtual IFileAccessor *open(const QString &fileName, int flags, QString &error) const;
	virtual IFileContainer *open(const QString &fileName, bool create, QString &error) const;

	virtual const IFileContainerScanner *scanner() const;

	/* IFileContainerScanner */
	virtual void enumerate(IEnumerator::Holder &enumerator) const;
	virtual IFileInfo *info(const QString &fileName, QString &error) const;
	virtual void scan(Snapshot &snapshot, const volatile Flags &aborted) const;
	virtual void refresh(Snapshot &snapshot, const volatile Flags &aborted) const;

private:
	void scan(WrappedNodeItem *root, const volatile Flags &aborted) const;

private:
	FileContainer(IFileContainer::Holder &container, IFileAccessor::Holder &file, const QString &fileName);

private:
	IFileContainer::Holder m_container;
	IFileAccessor::Holder m_file;
	QString m_fileName;
};

ARC_PLUGIN_NS_END

#endif /* ARCFILECONTAINER_H_ */
