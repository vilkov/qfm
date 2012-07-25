#ifndef DEFAULTFILECONTAINER_H_
#define DEFAULTFILECONTAINER_H_

#include "defaultfilecontainerscanner.h"


DEFAULT_PLUGIN_NS_BEGIN

class FileContainer : public IFileContainer
{
public:
	FileContainer(const QByteArray &path);

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
	virtual const IApplications *applications() const;

protected:
	Location m_path;
	FileContainerScanner m_scanner;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTFILECONTAINER_H_ */
