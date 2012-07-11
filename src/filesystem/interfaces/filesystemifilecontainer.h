#ifndef FILESYSTEMIFILECONTAINER_H_
#define FILESYSTEMIFILECONTAINER_H_

#include "filesystemifileaccessor.h"
#include "filesystemifilecontainerscanner.h"


FILE_SYSTEM_NS_BEGIN
class ICopyControl;
class INodeView;


class IFileContainer
{
public:
	typedef PScopedPointer<IFileContainer> Holder;

	class Filter
	{
	public:
		typedef PScopedPointer<Filter> Holder;

	public:
		virtual ~Filter();
		virtual bool match(const IFileInfo *info) const = 0;
	};

public:
	virtual ~IFileContainer();

	virtual bool isDefault() const = 0;
	virtual IFileInfo::size_type freeSpace() const = 0;
	virtual ICopyControl *createControl(INodeView *view) const = 0;

	virtual QString location() const = 0;
	virtual bool contains(const QString &fileName) const = 0;
	virtual IFileInfo *info(const QString &fileName, QString &error) const = 0;

	virtual bool remove(const IFileInfo *info, QString &error) const = 0;
	virtual bool rename(const IFileInfo *oldInfo, IFileInfo *newInfo, QString &error) const = 0;
	virtual bool move(const IFileContainer *source, const IFileInfo *info, QString &error) const = 0;

	virtual IFileContainer *open() const = 0;
	virtual IFileContainer *open(const IFileInfo *info, QString &error) const = 0;
	virtual IFileAccessor *open(const IFileInfo *info, int flags, QString &error) const = 0;
	virtual IFileContainer *create(const QString &fileName, QString &error) const = 0;
	virtual IFileAccessor *create(const QString &fileName, int flags, QString &error) const = 0;

	virtual IFileContainer *filter(Filter::Holder &filter, QString &error) const = 0;
	virtual const IFileContainerScanner *scanner() const = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMIFILECONTAINER_H_ */
