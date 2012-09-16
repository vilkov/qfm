#ifndef DEFAULT_FILECONTAINERSCANNER_H_
#define DEFAULT_FILECONTAINERSCANNER_H_

#include <vfs/interfaces/vfs_ifilecontainer.h>
#include "../default_ns.h"


DEFAULT_PLUGIN_NS_BEGIN
class SnapshotItem;


class FileContainerScanner : public IFileContainerScanner
{
public:
	FileContainerScanner(const IFileContainer *container);

	/* IFileContainerScanner */
	virtual IEnumerator *enumerate(QString &error) const;
	virtual void scan(const ScanArguments &arguments, QString &error) const;
	virtual void search(const SearchArguments &arguments, QString &error) const;

protected:
	void fill(Snapshot &snapshot, const volatile Flags &aborted, QString &error) const;
	void scan(SnapshotItem *root, const volatile Flags &aborted, QString &error) const;
	void search(const IFileContainer *container, const Callback &callback, const IFileContainerScanner::Filter *filter, const volatile Flags &aborted) const;

protected:
	const IFileContainer *m_container;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULT_FILECONTAINERSCANNER_H_ */
