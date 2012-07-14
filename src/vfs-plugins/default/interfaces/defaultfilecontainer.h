#ifndef DEFAULTFILECONTAINER_H_
#define DEFAULTFILECONTAINER_H_

#include "defaultfilecontainerscanner.h"


DEFAULT_PLUGIN_NS_BEGIN

class BaseFileContainer : public IFileContainer
{
public:
	BaseFileContainer(const QByteArray &path);

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

protected:
	friend class FileContainerScanner;
	friend class FilteredFileContainerScanner;
	Location m_path;
};


class FileContainer : public BaseFileContainer
{
public:
	FileContainer(const QByteArray &path);

	virtual const IFileContainerScanner *scanner() const;

private:
	FileContainerScanner m_scanner;
};


class FilteredFileContainer : public BaseFileContainer
{
public:
	FilteredFileContainer(const QByteArray &path, Filter::Holder &filter);

	virtual IFileInfo *info(const QString &fileName, QString &error) const;
	virtual const IFileContainerScanner *scanner() const;

private:
	Filter::Holder m_filter;
	FilteredFileContainerScanner m_scanner;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTFILECONTAINER_H_ */
