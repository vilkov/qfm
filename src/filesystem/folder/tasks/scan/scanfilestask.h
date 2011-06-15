#ifndef SCANFILESTASK_H_
#define SCANFILESTASK_H_

#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include <QtCore/QScopedPointer>
#ifndef Q_OS_WIN
#	include "../taskpermissionscache.h"
#endif
#include "../basetask.h"


FILE_SYSTEM_NS_BEGIN

/********************************************************************************************************/
template<typename BaseClass>
class ScanFilesTask : public BaseClass
{
public:
	typedef BaseClass parent_class;

public:
	struct Params : public parent_class::Params
	{
		Params(QObject *listener, const Info &node, const QStringList &entries) :
			source(listener, node, entries)
		{}

		Params(QObject *listener, const Info &node, const QStringList &entries, INode *destination) :
			parent_class::Params(destination),
			source(listener, node, entries)
		{}

		typename parent_class::Params::Snapshot source;
		QScopedPointer<FileSystemList> subnode;
		quint64 size;
	};

public:
	ScanFilesTask(Params *params) :
		parent_class(params)
	{}
	ScanFilesTask(Params *params, QObject *listener) :
		parent_class(params, listener)
	{}

	virtual void run(const volatile bool &stopedFlag)
	{
#ifndef Q_OS_WIN
		QScopedPointer<FileSystemList> subnode(new FileSystemList(m_permissions.getInfo(parameters()->source.node.absoluteFilePath(parameters()->source.entry))));
#else
		QScopedPointer<FileSystemList> subnode(new FileSystemList(parameters()->source.node.absoluteFilePath(parameters()->source.entry)));
#endif
		scan(subnode.data(), stopedFlag);
		parameters()->subnode.swap(subnode);
	}

protected:
	inline Params *parameters() const { return static_cast<Params*>(parent_class::parameters()); }

private:
	void scan(FileSystemList *node, const volatile bool &stopedFlag)
	{
		QFileInfo info;
		QDirIterator dirIt(node->absoluteFilePath(), QDir::AllEntries | QDir::System | QDir::Hidden | QDir::NoDotAndDotDot);

		while (!stopedFlag && !parent_class::isControllerDead() && dirIt.hasNext())
			if (!(info = dirIt.next()).isSymLink())
				if (info.isDir())
				{
#ifndef Q_OS_WIN
					QScopedPointer<FileSystemList> subtree(new FileSystemList(m_permissions.getInfo(info)));
#else
					QScopedPointer<FileSystemList> subtree(new FileSystemList(info));
#endif
					scan(subtree.data(), stopedFlag);
					node->add(subtree.take());
				}
				else
				{
#ifndef Q_OS_WIN
					node->add(new FileSystemEntry(m_permissions.getInfo(info)));
#else
					node->add(new FileSystemEntry(info));
#endif
					parameters()->size += info.size();
				}
	}

#ifndef Q_OS_WIN
private:
	TaskPermissionsCache m_permissions;
#endif
};

FILE_SYSTEM_NS_END

#endif /* SCANFILESTASK_H_ */
