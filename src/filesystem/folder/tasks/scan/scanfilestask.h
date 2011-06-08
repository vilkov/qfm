#ifndef SCANFILESTASK_H_
#define SCANFILESTASK_H_

#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include <QtCore/QScopedPointer>
#ifndef Q_OS_WIN
#	include "../taskpermissionscache.h"
#endif
#include "../../events/filesystemmodelevents.h"
#include "../../items/filesystemfoldernodeentry.h"
#include "../../items/filesystemfoldernodeitemlist.h"


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
		typename parent_class::Params::Snapshot source;
		QScopedPointer<FolderNodeItemList> subnode;
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
		QScopedPointer<FolderNodeItemList> subnode(new FolderNodeItemList(*parameters()->source.entry));
		scan(subnode.data(), stopedFlag);
		parameters()->subnode.swap(subnode);
	}

protected:
	inline Params *parameters() const { return static_cast<Params*>(parent_class::parameters()); }

private:
	void scan(FolderNodeItemList *node, const volatile bool &stopedFlag)
	{
		QFileInfo info;
		QDirIterator dirIt(node->absoluteFilePath(), QDir::AllEntries | QDir::System | QDir::Hidden | QDir::NoDotAndDotDot);

		while (!stopedFlag && !parent_class::isControllerDead() && dirIt.hasNext())
			if (!(info = dirIt.next()).isSymLink())
				if (info.isDir())
				{
#ifndef Q_OS_WIN
					QScopedPointer<FolderNodeItemList> subtree(new FolderNodeItemList(m_permissions.getInfo(info)));
#else
					QScopedPointer<FolderNodeItemList> subtree(new FolderNodeItemList(info));
#endif
					scan(subtree.data(), stopedFlag);
					node->add(subtree.take());
				}
				else
				{
#ifndef Q_OS_WIN
					node->add(new FolderNodeEntry(m_permissions.getInfo(info)));
#else
					node->add(new FolderNodeEntry(info));
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
