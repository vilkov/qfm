#ifndef FILESYSTEMIFILECONTAINERSCANNER_H_
#define FILESYSTEMIFILECONTAINERSCANNER_H_

#include "filesystemifileinfo.h"
#include "../../tools/taskspool/task.h"


FILE_SYSTEM_NS_BEGIN
class Snapshot;
class IFileAccessor;


class IFileContainerScanner
{
public:
	typedef ::Tools::TasksPool::Task::Flags Flags;

public:
	class IEnumerator
	{
	public:
		typedef PScopedPointer<IEnumerator> Holder;

	public:
		virtual ~IEnumerator();

		virtual bool next() = 0;
		virtual QString fileName() const = 0;
		virtual IFileInfo *info() const = 0;
		virtual bool isObsolete(const IFileInfo *item) const = 0;
		virtual IFileAccessor *open(int mode, QString &error) const = 0;
	};

public:
	virtual ~IFileContainerScanner();

	virtual void enumerate(IEnumerator::Holder &enumerator) const = 0;
	virtual IFileInfo *info(const QString &fileName, QString &error) const = 0;
	virtual void scan(Snapshot &snapshot, const volatile Flags &aborted, QString &error) const = 0;
	virtual void refresh(Snapshot &snapshot, const volatile Flags &aborted, QString &error) const = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMIFILECONTAINERSCANNER_H_ */
