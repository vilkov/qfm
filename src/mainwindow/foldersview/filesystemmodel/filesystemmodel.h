#ifndef FILESYSTEMMODEL_H_
#define FILESYSTEMMODEL_H_

#include <QtCore/QAbstractItemModel>
#include "filesysteminfo.h"
#include "filesystemchangeslist.h"
#include "items/filesystemitem.h"
#include "events/filesystemmodelevent.h"


class DirectoryView;

class FileSystemModel : public QAbstractItemModel
{
	Q_OBJECT
	Q_DISABLE_COPY(FileSystemModel)

public:
	FileSystemModel(const QString &currentDirectory, DirectoryView *parent);
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

	bool isLocked() const;
	QStringList lockedEntries() const;
	QModelIndex rootIndex() const;
	QModelIndex parentEntryIndex() const;
	const QFileInfo &currentDirectoryInfo() const;
	const QFileInfo &fileInfo(const QModelIndex &index) const;
	void pathToClipboard(const QModelIndexList &list) const;
	QModelIndex find(const QString &fileName) const;
	QString selectedFiles(const QModelIndexList &list, QStringList &files) const;

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
	void list(FileSystemItem *fileSystemTree);
	void listEvent(const FileSystemModelEvent::Params *p);

	void update(FileSystemItem *fileSystemTree);
	void updateEvent(const FileSystemModelEvent::Params *p);

	void removeEntry(FileSystemItem *fileSystemTree, FileSystemItem *entry);
	void scanForRemove(FileSystemItem *fileSystemTree, FileSystemItem *entry);
	void scanForRemoveEvent(const FileSystemModelEvent::Params *p);
	void removeCompleteEvent(const FileSystemModelEvent::Params *p);
	void removeCanceledEvent(const FileSystemModelEvent::Params *p);

	void scanForSize(FileSystemItem *fileSystemTree, FileSystemItem *entry);
	void scanForSizeEvent(const FileSystemModelEvent::Params *p);

	void copyEntry(FileSystemItem *fileSystemTree, FileSystemItem *entry, FileSystemModel *destination);
	void scanForCopy(FileSystemItem *fileSystemTree, FileSystemItem *entry, FileSystemModel *destination);
	void moveEntry(FileSystemItem *fileSystemTree, FileSystemItem *entry, FileSystemModel *destination);
	void scanForMove(FileSystemItem *fileSystemTree, FileSystemItem *entry, FileSystemModel *destination);
	void scanForCopyEvent(const FileSystemModelEvent::Params *p);
	void scanForMoveEvent(const FileSystemModelEvent::Params *p);
	void copyCompleteEvent(const FileSystemModelEvent::Params *p);
	void copyCanceledEvent(const FileSystemModelEvent::Params *p);

	void questionAnswerEvent(const FileSystemModelEvent::Params *p);
	void updateProgressEvent(const FileSystemModelEvent::Params *p);

protected:
	void updateFirstColumn(FileSystemItem *fileSystemTree, FileSystemItem *entry);
	void updateSecondColumn(FileSystemItem *fileSystemTree, FileSystemItem *entry);
	void updateBothColumns(FileSystemItem *fileSystemTree, FileSystemItem *entry);
	QModelIndex index(int column, FileSystemItem *item) const;
	QModelIndex index(int row, int column, FileSystemItem *parentItem) const;
	void removeEntry(FileSystemItem::size_type index);
	void removeEntry(const QModelIndex &index);
	void refresh(FileSystemItem *fileSystemTree);
	void doRefresh();

private:
	FileSystemItem *m_currentFsTree;
};

#endif /* FILESYSTEMMODEL_H_ */
