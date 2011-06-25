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
	struct Params : public DestControlableTask::Params
	{
		Params(QObject *listener, const Info &node, const QStringList &entries) :
			source(listener, node, entries)
		{}

		Snapshot source;
		QScopedPointer<FileSystemList> subnode;
	};

public:
	ScanFilesTask(Params *params, QObject *listener) :
		DestControlableTask(params, listener)
	{}

	virtual void run(const volatile bool &stopedFlag)
	{
		const QStringList &entries = parameters()->source.entries;
		QScopedPointer<FileSystemList> root(new FileSystemList(parameters()->source.info));

		for (QStringList::size_type i = 0, size = entries.size(); i < size && !stopedFlag; ++i)
		{
#ifndef Q_OS_WIN
			Info info(m_permissions.getInfo(root->absoluteFilePath(entries.at(i))));
#else
			Info info(root->absoluteFilePath(entries.at(i)));
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

		parameters()->subnode.swap(root);
	}

protected:
	inline Params *parameters() const { return static_cast<Params*>(DestControlableTask::parameters()); }

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

#ifndef Q_OS_WIN
private:
	TaskPermissionsCache m_permissions;
#endif
};

FILE_SYSTEM_NS_END

#endif /* SCANFILESTASK_H_ */
