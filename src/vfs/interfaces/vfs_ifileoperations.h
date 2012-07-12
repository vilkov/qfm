#ifndef VFS_IFILEOPERATIONS_H_
#define VFS_IFILEOPERATIONS_H_

#include <QtGui/QAction>
#include <QtCore/QModelIndexList>
#include "vfs_inodeview.h"
#include "vfs_icopycontrol.h"
#include "../../history/historyentry.h"


VFS_NS_BEGIN
class INode;


class IFileOperations
{
public:
	virtual ~IFileOperations();

	virtual ICopyControl *createControl(INodeView *view) const = 0;
	virtual void contextMenu(const QModelIndexList &list, INodeView *view) = 0;
	virtual void createFile(const QModelIndex &index, INodeView *view) = 0;
	virtual void createDirectory(const QModelIndex &index, INodeView *view) = 0;
	virtual void rename(const QModelIndex &index, INodeView *view) = 0;
	virtual void rename(const QModelIndexList &list, INodeView *view) = 0;
	virtual void remove(const QModelIndexList &list, INodeView *view) = 0;
	virtual void cancel(const QModelIndexList &list, INodeView *view) = 0;
	virtual void calculateSize(const QModelIndexList &list, INodeView *view) = 0;
	virtual void pathToClipboard(const QModelIndexList &list, INodeView *view) = 0;
	virtual void copy(const INodeView *source, INodeView *destination) = 0;
	virtual void move(const INodeView *source, INodeView *destination) = 0;
	virtual void removeToTrash(const QModelIndexList &list, INodeView *view) = 0;
	virtual ::History::Entry *search(const QModelIndex &index, INodeView *view) = 0;
};

VFS_NS_END

#endif /* VFS_IFILEOPERATIONS_H_ */
