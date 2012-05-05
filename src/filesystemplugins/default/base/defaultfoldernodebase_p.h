#ifndef DEFAULTFOLDERNODEBASE_P_H_
#define DEFAULTFOLDERNODEBASE_P_H_


DEFAULT_PLUGIN_NS_BEGIN

class FolderNodeBase::ScanForSizeEventFunctor : public EventFunctor
{
public:
	ScanForSizeEventFunctor()
	{}

	Union updateRange;

protected:
	virtual void call(Container::size_type index, DefaultNodeItem *item, WrappedNodeItem *entry)
	{
		updateRange.add(index);
		item->setTotalSize(entry->totalSize());
		item->unlock();
	}
};


class FolderNodeBase::ScanForSizeEventFunctor_canceled : public EventFunctor
{
public:
	ScanForSizeEventFunctor_canceled()
	{}

	Union updateRange;

protected:
	virtual void call(Container::size_type index, DefaultNodeItem *item, WrappedNodeItem *entry)
	{
		updateRange.add(index);
		item->clearTotalSize();
		item->unlock();
	}
};


class FolderNodeBase::ScanForCopyEventFunctor : public EventFunctor
{
public:
	ScanForCopyEventFunctor(const QString &lockReason) :
		lockReason(lockReason)
	{}

	QString lockReason;
	Union updateRange;

protected:
	virtual void call(Container::size_type index, DefaultNodeItem *item, WrappedNodeItem *entry)
	{
		updateRange.add(index);
		item->lock(lockReason, entry->totalSize());
	}
};


class FolderNodeBase::ScanForCopyEventFunctor_canceled : public EventFunctor
{
public:
	ScanForCopyEventFunctor_canceled()
	{}

	Union updateRange;

protected:
	virtual void call(Container::size_type index, DefaultNodeItem *item, WrappedNodeItem *entry)
	{
		updateRange.add(index);
		item->setTotalSize(entry->totalSize());
		item->unlock();
	}
};


class FolderNodeBase::ScanForRemoveEventFunctor : public EventFunctor
{
public:
	ScanForRemoveEventFunctor(const QString &lockReason) :
		lockReason(lockReason)
	{}

	QString lockReason;
	Union updateRange;
	QStringList folders;
	QStringList files;

protected:
	virtual void call(Container::size_type index, DefaultNodeItem *item, WrappedNodeItem *entry)
	{
		updateRange.add(index);

		if (entry->info()->isDir())
			folders.push_back(entry->info()->fileName());
		else
			files.push_back(entry->info()->fileName());

		item->lock(lockReason, entry->totalSize());
	}
};


class FolderNodeBase::ScanForRemoveEventFunctor_canceled : public EventFunctor
{
public:
	ScanForRemoveEventFunctor_canceled()
	{}

	Union updateRange;

protected:
	virtual void call(Container::size_type index, DefaultNodeItem *item, WrappedNodeItem *entry)
	{
		updateRange.add(index);
		item->setTotalSize(entry->totalSize());
		item->unlock();
	}
};


class FolderNodeBase::PerformCopyEventFunctor : public EventFunctor
{
public:
	PerformCopyEventFunctor(const QString &lockReason) :
		lockReason(lockReason)
	{}

	QString lockReason;
	Union updateRange;

protected:
	virtual void call(Container::size_type index, DefaultNodeItem *item, WrappedNodeItem *entry)
	{
		updateRange.add(index);

		if (entry->info()->isDir())
			item->lock(lockReason, entry->totalSize());
		else
			item->lock(lockReason);
	}
};


class FolderNodeBase::PerformCopyEventFunctor_canceled : public EventFunctor
{
public:
	PerformCopyEventFunctor_canceled()
	{}

	Union updateRange;

protected:
	virtual void call(Container::size_type index, DefaultNodeItem *item, WrappedNodeItem *entry)
	{
		updateRange.add(index);
		item->unlock();
	}
};


class FolderNodeBase::PerformRemoveEventFunctor : public EventFunctor
{
public:
	typedef void (FolderNodeBase::*Method)(Container::size_type index);

public:
	PerformRemoveEventFunctor(FolderNodeBase *node, Method method) :
		node(node),
		method(method)
	{}

	mutable QStringList list;
	FolderNodeBase *node;
	Method method;

protected:
	virtual void call(Container::size_type index, DefaultNodeItem *item, WrappedNodeItem *entry)
	{
		if (entry->isRemoved())
			(node->*method)(index);
		else
		{
			list.push_back(item->info()->fileName());
			item->clearTotalSize();
			item->unlock();
		}
	}
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTFOLDERNODEBASE_P_H_ */
