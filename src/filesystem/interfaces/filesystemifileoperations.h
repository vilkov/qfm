#ifndef FILESYSTEMIFILEOPERATIONS_H_
#define FILESYSTEMIFILEOPERATIONS_H_

#include <QtGui/QAction>
#include <QtCore/QModelIndexList>
#include "../filesystem_ns.h"


FILE_SYSTEM_NS_BEGIN

class INode;

class IFileOperations
{
public:
	virtual ~IFileOperations() {}

	virtual void menuAction(QAction *action) = 0;
	virtual void createFile(const QModelIndex &index) = 0;
	virtual void createDirectory(const QModelIndex &index) = 0;
	virtual void rename(const QModelIndexList &list) = 0;
	virtual void remove(const QModelIndexList &list) = 0;
	virtual void cancel(const QModelIndexList &list) = 0;
	virtual void calculateSize(const QModelIndexList &list) = 0;
	virtual void pathToClipboard(const QModelIndexList &list) = 0;
	virtual void copy(const QModelIndexList &list, INode *destination) = 0;
	virtual void move(const QModelIndexList &list, INode *destination) = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMIFILEOPERATIONS_H_ */
