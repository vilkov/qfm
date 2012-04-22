#ifndef FILESYSTEMCOPYINFO_H_
#define FILESYSTEMCOPYINFO_H_

#include <QtCore/QCoreApplication>
#include "filesystemfilecontainer.h"
#include "../../tasks/interfaces/filesystemicopycontrol.h"


FILE_SYSTEM_NS_BEGIN

class CopyControl : public ICopyControl
{
	Q_DECLARE_TR_FUNCTIONS(CopyControl)

public:
	CopyControl(INode *node, const QString &path);

	/* IFileContainer */
	virtual bool isPhysical() const;
	virtual QString location() const;
	virtual QString location(const QString &fileName) const;
	virtual IFileInfo::size_type freeSpace() const;

	virtual bool contains(const QString &fileName) const;
	virtual bool remove(const QString &fileName, QString &error) const;
	virtual bool rename(const QString &oldName, const QString &newName, QString &error) const;

	virtual IFileContainer *open() const;
	virtual IFileAccessor *open(const QString &fileName, int mode, QString &error) const;
	virtual IFileContainer *open(const QString &fileName, bool create, QString &error) const;

	/* ICopyControl */
	virtual INode *node() const;
	virtual bool start(const Snapshot::Files &files, bool move);
	virtual void done(bool error);
	virtual void canceled();

private:
	INode *m_node;
	FileContainer m_container;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMCOPYINFO_H_ */
