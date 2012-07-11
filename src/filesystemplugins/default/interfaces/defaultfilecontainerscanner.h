#ifndef DEFAULTFILECONTAINERSCANNER_H_
#define DEFAULTFILECONTAINERSCANNER_H_

#include "../default_ns.h"
#include "../../../filesystem/interfaces/filesystemifilecontainer.h"


DEFAULT_PLUGIN_NS_BEGIN
class WrappedNodeItem;


class FileContainerScanner : public IFileContainerScanner
{
public:
	FileContainerScanner(const IFileContainer *container);

	/* IFileContainerScanner */
	virtual IEnumerator *enumerate(QString &error) const;
	virtual void scan(Snapshot &snapshot, const volatile Flags &aborted, QString &error) const;
	virtual void refresh(Snapshot &snapshot, const volatile Flags &aborted, QString &error) const;

protected:
	virtual void fill(Snapshot &snapshot, const volatile Flags &aborted, QString &error) const;
	virtual void scan(WrappedNodeItem *root, const volatile Flags &aborted, QString &error) const;

protected:
	const IFileContainer *m_container;
};


class FilteredFileContainerScanner : public FileContainerScanner
{
public:
	FilteredFileContainerScanner(const IFileContainer *container, const IFileContainer::Filter *filter);

	/* IFileContainerScanner */
	virtual IEnumerator *enumerate(QString &error) const;

protected:
	virtual void fill(Snapshot &snapshot, const volatile Flags &aborted, QString &error) const;
	virtual void scan(WrappedNodeItem *root, const volatile Flags &aborted, QString &error) const;

private:
	const IFileContainer::Filter *m_filter;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTFILECONTAINERSCANNER_H_ */
