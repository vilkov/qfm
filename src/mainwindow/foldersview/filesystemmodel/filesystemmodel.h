#ifndef FILESYSTEMMODEL_H_
#define FILESYSTEMMODEL_H_

#include <QAbstractItemModel>
#include <QFileIconProvider>
#include "filesystemmodeladaptor.h"
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
	void pathToClipboard(const FileSystemModelAdaptor &list) const;
	QModelIndex find(const QString &fileName) const;
	void copy(const QModelIndex &index, FileSystemModel *destination) const;

	void refresh();
	void refreshSize(const QModelIndex &index);
	void activated(const QModelIndex &index);
	void setCurrentDirectory(const QString &filePath);
	void setCurrentDirectory(const QFileInfo &info);
	void rename(const QModelIndex &index, const QString &newFileName);
	void createDirectory(const QString &dirName);
	void remove(const QModelIndex &index);

protected:
	void list(FileSystemItem *fileSystemTree);
	void listEvent(const FileSystemModelEvent::Params *p);

	void update(FileSystemItem *fileSystemTree);
	void updateEvent(const FileSystemModelEvent::Params *p);

	void scanForRemove(FileSystemItem *fileSystemTree, FileSystemItem *entry);
	void scanForRemoveEvent(const FileSystemModelEvent::Params *p);

	void scanForSize(FileSystemItem *fileSystemTree, FileSystemItem *entry);
	void scanForSizeEvent(const FileSystemModelEvent::Params *p);

protected:
	bool isLocked() const;
	QModelIndex index(int column, FileSystemItem *item) const;
	QModelIndex index(int row, int column, FileSystemItem *parentItem) const;

private:
	FileSystemItem *m_currentFsTree;
};

#endif /* FILESYSTEMMODEL_H_ */
