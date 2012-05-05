#ifndef DEFAULTCOPYINFO_H_
#define DEFAULTCOPYINFO_H_

#include <QtCore/QCoreApplication>
#include "defaultfilecontainer.h"
#include "../../../filesystem/interfaces/filesystemicopycontrol.h"


DEFAULT_PLUGIN_NS_BEGIN

class CopyControl : public ICopyControl
{
	Q_DECLARE_TR_FUNCTIONS(CopyControl)

public:
	CopyControl(INode *node, const QString &path);

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

	/* ICopyControl */
	virtual INode *node() const;
	virtual bool start(const Snapshot &files, bool move);
	virtual void done(bool error);
	virtual void canceled();

private:
	INode *m_node;
	FileContainer m_container;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTCOPYINFO_H_ */
