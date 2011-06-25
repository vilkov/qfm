#ifndef SCANFILESTASK_H_
#define SCANFILESTASK_H_

#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include <QtCore/QScopedPointer>
#ifndef Q_OS_WIN
#	include "../taskpermissionscache.h"
#endif
#include "../destcontrolabletask.h"


FILE_SYSTEM_NS_BEGIN

class ScanFilesTask : public DestControlableTask
{
public:
	ScanFilesTask(QObject *receiver, const Info &info, const QStringList &entries) :
		DestControlableTask(receiver),
		m_info(info),
		m_entries(entries)
	{}

	virtual void run(const volatile bool &stopedFlag)
	{
		QScopedPointer<FileSystemList> root(new FileSystemList(m_info));

		for (QStringList::size_type i = 0, size = m_entries.size(); i < size && !stopedFlag; ++i)
		{
#ifndef Q_OS_WIN
			Info info(m_permissions.getInfo(root->absoluteFilePath(m_entries.at(i))));
#else
			Info info(root->absoluteFilePath(m_entries.at(i)));
#endif

			if (info.exists())
				if (info.isDir())
				{
					QScopedPointer<FileSystemList> subnode(new FileSystemList(info));

					scan(subnode.data(), stopedFlag);
					root->incTotalSize(subnode->totalSize());
					root->add(subnode.take());
				}
				else
				{
					root->add(new FileSystemEntry(info));
					root->incTotalSize(info.size());
				}
			else
				root->add(new FileSystemEntry(info));
		}

		m_subnode.swap(root);
	}

protected:
	const QScopedPointer<FileSystemList> &subnode() const { return m_subnode; }
	QScopedPointer<FileSystemList> &subnode() { return m_subnode; }

private:
	void scan(FileSystemList *node, const volatile bool &stopedFlag)
	{
		QFileInfo info;
		QDirIterator dirIt(node->absoluteFilePath(), QDir::AllEntries | QDir::System | QDir::Hidden | QDir::NoDotAndDotDot);

		while (!stopedFlag && !isControllerDead() && dirIt.hasNext())
			if (!(info = dirIt.next()).isSymLink())
				if (info.isDir())
				{
#ifndef Q_OS_WIN
					QScopedPointer<FileSystemList> subtree(new FileSystemList(m_permissions.getInfo(info)));
#else
					QScopedPointer<FileSystemList> subtree(new FileSystemList(info));
#endif
					scan(subtree.data(), stopedFlag);
					node->incTotalSize(subtree->totalSize());
					node->add(subtree.take());
				}
				else
				{
#ifndef Q_OS_WIN
					node->add(new FileSystemEntry(m_permissions.getInfo(info)));
#else
					node->add(new FileSystemEntry(info));
#endif
					node->incTotalSize(info.size());
				}
	}

private:
	Info m_info;
	QStringList m_entries;
	QScopedPointer<FileSystemList> m_subnode;

#ifndef Q_OS_WIN
private:
	TaskPermissionsCache m_permissions;
#endif
};

FILE_SYSTEM_NS_END

#endif /* SCANFILESTASK_H_ */
