#ifndef FILESYSTEMFOLDERNODEBASE_H_
#define FILESYSTEMFOLDERNODEBASE_H_

#include <QtCore/QStringList>
#include "events/filesystemmodelevent.h"
#include "containers/filesystemtasksmap.h"
#include "containers/filesystemupdateslist.h"
#include "containers/filesystemitemscontainer.h"
#include "../../filesystemnode.h"
#include "../../../tools/pointers/pscopedpointer.h"


FILE_SYSTEM_NS_BEGIN

/*
 * Implements general file system tasks (coping, moving, removing etc).
 *
 */

class FolderNodeBase : public Node
{
	Q_DISABLE_COPY(FolderNodeBase)

public:
	FolderNodeBase(const Info &info, Node *parent = 0);

	/* FileSystemModel */
    virtual bool event(QEvent *event);

    /* INode */
	virtual ICopyControl *createControl() const;

	/* INode::IFileInfo */
	virtual bool isDir() const;
	virtual bool isFile() const;
	virtual bool exists() const;
	virtual QString fileName() const;
	virtual QString absolutePath() const;
	virtual QString absoluteFilePath() const;
	virtual QString absoluteFilePath(const QString &fileName) const;
	virtual QDateTime lastModified() const;
	virtual void refresh();

protected:
	/* Node */
	virtual void removeChild(Node *node);

protected:
	/* Tasks events */
	virtual UpdatesList::Map updateFilesMap() const = 0;
	virtual void updateFilesEvent(const UpdatesList &updates) = 0;
	virtual void scanForSizeEvent(bool canceled, PScopedPointer<InfoListItem> &entries) = 0;
	virtual void scanForCopyEvent(bool canceled, PScopedPointer<InfoListItem> &entries, PScopedPointer<ICopyControl> &control, bool move) = 0;
	virtual void scanForRemoveEvent(bool canceled, PScopedPointer<InfoListItem> &entries) = 0;
	virtual void performCopyEvent(bool canceled, PScopedPointer<InfoListItem> &entries, bool move) = 0;
	virtual void performRemoveEvent(PScopedPointer<InfoListItem> &entries) = 0;

	virtual void updateProgressEvent(const QString &fileName, quint64 progress, quint64 timeElapsed) = 0;
	virtual void completedProgressEvent(const QString &fileName, quint64 timeElapsed) = 0;

protected:
	/* Prepare tasks */
	void scanForSize(const QStringList &entries);
	void scanForCopy(const QStringList &entries, PScopedPointer<ICopyControl> &control, bool move);
	void scanForRemove(const QStringList &entries);
	void performCopy(PScopedPointer<InfoListItem> &entries, PScopedPointer<ICopyControl> &control, bool move);
	void performRemove(PScopedPointer<InfoListItem> &entries);

protected:
	bool isRoot() const { return m_info.isRoot(); }
	bool isUpdating() const { return m_updating; }
	void setUpdating(bool value) { m_updating = value; }

	IFileControl *create(const QString &name, IFileControl::FileType type, QString &error) const { return m_info.create(name, type, error); }

	const TasksMap &tasks() const { return m_tasks; }
	TasksMap &tasks() { return m_tasks; }

	ItemsContainer::size_type size() const { return m_items.m_items.size(); }
	ItemsContainer::size_type indexOf(const QString &fileName) const { return m_items.m_items.indexOf(fileName); }
	FileSystemBaseItem *at(ItemsContainer::size_type index) const { return static_cast<FileSystemBaseItem *>(m_items.m_items.at(index)); }
	void add(FileSystemBaseItem *item);
	void remove(FileSystemBaseItem *item);
	void remove(ItemsContainer::size_type index);

private:
	void updateFiles();
	void updateFiles(const ModelEvent *event);
	void scanForSize(const ModelEvent *event);
	void scanForCopy(const ModelEvent *event);
	void scanForRemove(const ModelEvent *event);
	void performCopy(const ModelEvent *event);
	void performRemove(const ModelEvent *event);

	void questionAnswer(const ModelEvent *event);
	void updateProgress(const ModelEvent *event);
	void completedProgress(const ModelEvent *event);

private:
	typedef QList<Node::Holder> Nodes;

private:
	bool m_updating;
	Info m_info;
	TasksMap m_tasks;
	Nodes m_nodes;
	ItemsContainer m_items;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERNODEBASE_H_ */
