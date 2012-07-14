#ifndef DEFAULTCOPYINFO_H_
#define DEFAULTCOPYINFO_H_

#include <QtCore/QCoreApplication>
#include <vfs/interfaces/vfs_icopycontrol.h>
#include "defaultfilecontainer.h"


DEFAULT_PLUGIN_NS_BEGIN

class CopyControl : public ICopyControl
{
	Q_DECLARE_TR_FUNCTIONS(CopyControl)

public:
	CopyControl(INode *node, const QByteArray &path);

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

	virtual IFileContainer *filter(Filter::Holder &filter, QString &error) const;
	virtual const IFileContainerScanner *scanner() const;

	/* ICopyControl */
	virtual INode *node() const;
	virtual bool start(const Snapshot &files, bool move);
	virtual void done(bool error);
	virtual void canceled();

private:
	FileContainer m_container;
	INode *m_node;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTCOPYINFO_H_ */
