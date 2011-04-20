#ifndef FILESYSTEMMODEL_H_
#define FILESYSTEMMODEL_H_

#include <QAbstractItemModel>
#include "filesysteminfo.h"
#include "filesystemchangeslist.h"
#include "items/filesystemitem.h"
#include "events/filesystemmodelevents.h"


class FileSystemModel : public QAbstractItemModel
{
	Q_OBJECT
	Q_DISABLE_COPY(FileSystemModel)

public:
	FileSystemModel(const QString &currentDirectory, QObject *parent = 0);
	FileSystemModel(const QFileInfo &fileInfo, QObject *parent = 0);
	~FileSystemModel();

    virtual bool event(QEvent *e);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &child) const;

	QModelIndex rootIndex() const;
	QModelIndex parentEntryIndex() const;
	const QFileInfo &currentDirectoryInfo() const;
	const QFileInfo &fileInfo(const QModelIndex &index) const;
	void pathToClipboard(const QModelIndexList &list) const;
	QModelIndex find(const QString &fileName) const;

	void refresh();
	void refreshSize(const QModelIndex &index);
	void activated(const QModelIndex &index);
	QModelIndex setCurrentDirectory(const QString &filePath);
	QModelIndex setCurrentDirectory(const QFileInfo &info);
	void rename(const QModelIndex &index, const QString &newFileName);
	void createDirectory(const QString &dirName);
	void remove(const QModelIndex &index);
	void copy(const QModelIndex &index, FileSystemModel *destination);
	void move(const QModelIndex &index, FileSystemModel *destination);

protected:
	void firstList(FileSystemItem *fileSystemTree);
	void firstListEvent(const FileSystemModelEvent::EventParams *p);
	void list(FileSystemItem *fileSystemTree);
	void listEvent(const FileSystemModelEvent::EventParams *p);

	void update(FileSystemItem *fileSystemTree);
	void updateEvent(const FileSystemModelEvent::EventParams *p);

	void removeEntry(FileSystemItem *fileSystemTree, FileSystemItem *entry);
	void scanForRemove(FileSystemItem *fileSystemTree, FileSystemItem *entry);
	void scanForRemoveEvent(const FileSystemModelEvent::EventParams *p);
	void removeCompleteEvent(const FileSystemModelEvent::EventParams *p);
	void removeCanceledEvent(const FileSystemModelEvent::EventParams *p);

	void scanForSize(FileSystemItem *fileSystemTree, FileSystemItem *entry);
	void scanForSizeEvent(const FileSystemModelEvent::EventParams *p);

	void copyEntry(FileSystemItem *fileSystemTree, FileSystemItem *entry, FileSystemModel *destination);
	void scanForCopy(FileSystemItem *fileSystemTree, FileSystemItem *entry, FileSystemModel *destination);
	void moveEntry(FileSystemItem *fileSystemTree, FileSystemItem *entry, FileSystemModel *destination);
	void scanForMove(FileSystemItem *fileSystemTree, FileSystemItem *entry, FileSystemModel *destination);
	void scanForCopyEvent(const FileSystemModelEvent::EventParams *p);
	void scanForMoveEvent(const FileSystemModelEvent::EventParams *p);
	void copyCompleteEvent(const FileSystemModelEvent::EventParams *p);
	void copyCanceledEvent(const FileSystemModelEvent::EventParams *p);

	void questionAnswerEvent(const FileSystemModelEvent::EventParams *p);
	void newEntryEvent(const FileSystemModelEvent::EventParams *p);

protected:
	bool isLocked() const;
	void updateFirstColumn(FileSystemItem *fileSystemTree, FileSystemItem *entry);
	void updateSecondColumn(FileSystemItem *fileSystemTree, FileSystemItem *entry);
	void updateBothColumns(FileSystemItem *fileSystemTree, FileSystemItem *entry);
	QModelIndex index(int column, FileSystemItem *item) const;
	QModelIndex index(int row, int column, FileSystemItem *parentItem) const;
	void removeEntry(FileSystemItem::size_type index);
	void removeEntry(const QModelIndex &index);

private:
	FileSystemItem *m_currentFsTree;
};

#endif /* FILESYSTEMMODEL_H_ */
