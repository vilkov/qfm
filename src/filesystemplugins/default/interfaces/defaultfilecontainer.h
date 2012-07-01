#ifndef DEFAULTFILECONTAINER_H_
#define DEFAULTFILECONTAINER_H_

#include "defaultfilecontainerscanner.h"


DEFAULT_PLUGIN_NS_BEGIN

class BaseFileContainer : public IFileContainer
{
public:
	BaseFileContainer(const QString &path);

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
	virtual IFileContainer *filter(Filter::Holder &filter, QString &error) const;

private:
	QString m_path;
};


class FileContainer : public BaseFileContainer
{
public:
	FileContainer(const QString &path);

	virtual const IFileContainerScanner *scanner() const;

private:
	FileContainerScanner m_scanner;
};


class FilteredFileContainer : public BaseFileContainer
{
public:
	FilteredFileContainer(const QString &path, Filter::Holder &filter);

	virtual const IFileContainerScanner *scanner() const;

private:
	FilteredFileContainerScanner m_scanner;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTFILECONTAINER_H_ */
