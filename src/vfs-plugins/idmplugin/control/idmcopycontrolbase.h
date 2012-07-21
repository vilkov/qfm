#ifndef IDMCOPYCONTROLBASE_H_
#define IDMCOPYCONTROLBASE_H_

#include <vfs/interfaces/vfs_icopycontrol.h>
#include "../containeres/idmcontainer.h"


IDM_PLUGIN_NS_BEGIN

class IdmCopyControlBase : public ICopyControl
{
public:
	IdmCopyControlBase(ICopyControl::Holder &dest, const IdmContainer &container);

	/* IFileContainer */
	virtual bool isDefault() const;
	virtual IFileInfo::size_type freeSpace() const;
	virtual ICopyControl *createControl(INodeView *view) const;

	virtual const Location &location() const;
	virtual Location location(const IFileInfo *info) const;
	virtual Location location(const QString &fileName) const;

	virtual bool contains(const QString &fileName) const;
	virtual IFileInfo *info(const QString &fileName, QString &error) const;

	virtual bool remove(const IFileInfo *info, QString &error) const;
	virtual bool rename(const IFileInfo *info, const QString &fileName, QString &error) const;
	virtual bool move(const IFileContainer *source, const IFileInfo *info, QString &error) const;

	virtual IFileContainer *open() const;
	virtual IFileContainer *open(const IFileInfo *info, QString &error) const;
	virtual IFileAccessor *open(const IFileInfo *info, int flags, QString &error) const;
	virtual IFileContainer *create(const QString &fileName, QString &error) const;
	virtual IFileAccessor *create(const QString &fileName, int flags, QString &error) const;

	virtual const IFileContainerScanner *scanner() const;

	/* ICopyControl */
	virtual INode *node() const;

	virtual bool start(const Snapshot &files, bool move);
	virtual void done(bool error);
	virtual void canceled();

protected:
	static QString difference(const QString &path1, const QString &path2);

protected:
	ICopyControl::Holder m_dest;
	IdmContainer m_container;
	QString m_storage;
};

IDM_PLUGIN_NS_END

#endif /* IDMCOPYCONTROLBASE_H_ */
