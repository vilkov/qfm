#ifndef FILESYSTEMCOPYINFO_H_
#define FILESYSTEMCOPYINFO_H_

#include <QtCore/QCoreApplication>
#include "filesystemfilecontainer.h"
#include "../tasks/interfaces/filesystemicopycontrol.h"


FILE_SYSTEM_NS_BEGIN

class CopyControl : public ICopyControl
{
	Q_DECLARE_TR_FUNCTIONS(CopyControl)

public:
	CopyControl(const Info &info);

	/* IFileContainer */
	virtual bool isPhysical() const;
	virtual QString location() const;
	virtual IFileInfo::size_type freeSpace() const;

	virtual bool contains(const QString &fileName) const;
	virtual bool remove(const QString &fileName, QString &error) const;

	virtual IFile *open(const QString &fileName) const;
	virtual IFileContainer *open(const QString &fileName, bool create, QString &error) const;

	virtual bool copy(const IFileInfo *source, QString &error) const;

	/* ICopyControl */
	virtual bool start(const ScanedFiles::Files &files, bool move);
	virtual void done(bool error);
	virtual void canceled();

protected:
	const Info &info() const { return m_container.info(); }
	Info &info() { return m_container.info(); }

private:
	FileContainer m_container;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMCOPYINFO_H_ */
