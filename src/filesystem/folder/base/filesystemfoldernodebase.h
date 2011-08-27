#ifndef FILESYSTEMFOLDERNODEBASE_H_
#define FILESYSTEMFOLDERNODEBASE_H_

#include "tasks/basetask.h"
#include "containers/filesystemtasksmap.h"
#include "containers/filesystemupdateslist.h"
#include "../../filesystemnode.h"


FILE_SYSTEM_NS_BEGIN

class FolderNodeBase : public Node
{
	Q_DISABLE_COPY(FolderNodeBase)

public:
	FolderNodeBase(const Info &info, Node *parent = 0);

	/* QAbstractItemModel */
    virtual bool event(QEvent *event);

    /* INode */
	virtual IFileControl *acceptCopy(const FileInfoList &files, bool move) const;

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
	/* Tasks events */
	virtual UpdatesList::Map updateFilesMap() const = 0;
	virtual void updateFilesEvent(const UpdatesList &updates) = 0;
	virtual void scanForSizeEvent(bool canceled, PScopedPointer<FileSystemList> &entries) = 0;
	virtual void scanForCopyEvent(bool canceled, PScopedPointer<FileSystemList> &entries, PScopedPointer<IFileControl> &destination, bool move) = 0;
	virtual void scanForRemoveEvent(bool canceled, PScopedPointer<FileSystemList> &entries) = 0;
	virtual void performCopyEvent(bool canceled, PScopedPointer<FileSystemList> &entries, bool move) = 0;
	virtual void performRemoveEvent(PScopedPointer<FileSystemList> &entries) = 0;

	virtual void updateProgressEvent(const QString &fileName, quint64 progress, quint64 timeElapsed) = 0;
	virtual void completedProgressEvent(const QString &fileName, quint64 timeElapsed) = 0;

protected:
	/* Prepare tasks */
	void scanForSize(const FileInfoList &entries);
	void scanForCopy(const FileInfoList &entries, INode *destination, bool move);
	void scanForRemove(const FileInfoList &entries);
	void performCopy(PScopedPointer<FileSystemList> &entries, PScopedPointer<IFileControl> &destination, bool move);
	void performRemove(PScopedPointer<FileSystemList> &entries);

protected:
	bool isRoot() const { return m_info.isRoot(); }
	bool isUpdating() const { return m_updating; }
	void setUpdating(bool value) { m_updating = value; }

	IFileControl *create(const QString &name, IFileControl::FileType type, QString &error) const { return m_info.create(name, type, error); }

	const TasksMap &tasks() const { return m_tasks; }
	TasksMap &tasks() { return m_tasks; }

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
	bool m_updating;
	Info m_info;
	TasksMap m_tasks;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERNODEBASE_H_ */
