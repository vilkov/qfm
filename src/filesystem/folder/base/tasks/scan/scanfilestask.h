#ifndef SCANFILESTASK_H_
#define SCANFILESTASK_H_

#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#ifndef Q_OS_WIN
#	include "../taskpermissionscache.h"
#endif
#include "../destcontrolabletask.h"


FILE_SYSTEM_NS_BEGIN

class ScanFilesTask : public DestControlableTask
{
public:
	class Event : public DestControlableTask::Event
	{
	public:
		Event(Type type, bool canceled, PScopedPointer<InfoListItem> &entries) :
			DestControlableTask::Event(type, canceled),
			entries(entries.take())
		{}

		PScopedPointer<InfoListItem> entries;
	};

public:
	ScanFilesTask(QObject *receiver, const Info &info, const EntryList &entries) :
		DestControlableTask(receiver),
		m_info(info),
		m_entries(entries)
	{}

	virtual void run(const volatile bool &aborted)
	{
		PScopedPointer<InfoListItem> root(new InfoListItem(m_info));

		for (QStringList::size_type i = 0, size = m_entries.size(); i < size && !aborted; ++i)
		{
#ifndef Q_OS_WIN
			Info info(m_permissions.getInfo(root->absoluteFilePath(m_entries.at(i))));
#else
			Info info(root->absoluteFilePath(m_entries.at(i)));
#endif

			if (info.exists())
				if (info.isDir())
				{
					PScopedPointer<InfoListItem> subnode(new InfoListItem(info));

					scan(subnode.data(), aborted);
					root->incTotalSize(subnode->totalSize());
					root->add(subnode.take());
				}
				else
				{
					root->add(new InfoEntryItem(info));
					root->incTotalSize(info.size());
				}
			else
				root->add(new InfoEntryItem(info));
		}

		m_subnode.swap(root);
	}

protected:
	const PScopedPointer<InfoListItem> &subnode() const { return m_subnode; }
	PScopedPointer<InfoListItem> &subnode() { return m_subnode; }

private:
	void scan(InfoListItem *node, const volatile bool &aborted)
	{
		QFileInfo info;
		QDirIterator dirIt(node->absoluteFilePath(), QDir::AllEntries | QDir::System | QDir::Hidden | QDir::NoDotAndDotDot);

		while (!isCanceled() && !aborted && !isControllerDead() && dirIt.hasNext())
			if (!(info = dirIt.next()).isSymLink())
				if (info.isDir())
				{
#ifndef Q_OS_WIN
					PScopedPointer<InfoListItem> subtree(new InfoListItem(m_permissions.getInfo(info)));
#else
					PScopedPointer<InfoListItem> subtree(new InfoListItem(info));
#endif
					scan(subtree.data(), aborted);
					node->incTotalSize(subtree->totalSize());
					node->add(subtree.take());
				}
				else
				{
#ifndef Q_OS_WIN
					node->add(new InfoEntryItem(m_permissions.getInfo(info)));
#else
					node->add(new InfoEntryItem(info));
#endif
					node->incTotalSize(info.size());
				}
	}

private:
	Info m_info;
	EntryList m_entries;
	PScopedPointer<InfoListItem> m_subnode;

#ifndef Q_OS_WIN
private:
	TaskPermissionsCache m_permissions;
#endif
};

FILE_SYSTEM_NS_END

#endif /* SCANFILESTASK_H_ */
