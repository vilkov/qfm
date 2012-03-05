#ifndef FILESYSTEMFOLDERNODEBASE_H_
#define FILESYSTEMFOLDERNODEBASE_H_

#include "containers/filesystemupdateslist.h"
#include "../../info/filesystemfilecontainer.h"
#include "../../tasks/filesystemtasksnode.h"
#include "../../tasks/concrete/containers/filesystemsnapshot.h"


FILE_SYSTEM_NS_BEGIN

/*
 * Implements interaction with file system tasks (coping, moving, removing etc).
 *
 */

class FolderNodeBase : public TasksNode, public FileContainer
{
	Q_DISABLE_COPY(FolderNodeBase)

public:
	FolderNodeBase(const Info &info, const ModelContainer &conteiner, Node *parent = 0);

	/* TasksNode */
    virtual bool event(QEvent *event);

	/* IFileType */
	virtual FileTypeId id() const;
	virtual QIcon icon() const;
	virtual QString name() const;
	virtual QString description() const;

	/* IFileInfo */
	virtual bool isDir() const;
	virtual bool isFile() const;
	virtual bool isLink() const;
	virtual bool exists() const;
	virtual size_type fileSize() const;
	virtual QString fileName() const;
	virtual QString absolutePath() const;
	virtual QString absoluteFilePath() const;
	virtual QString absoluteFilePath(const QString &fileName) const;
	virtual QDateTime lastModified() const;
	virtual int permissions() const;
	virtual void refresh();

    /* IFileOperations */
	virtual ICopyControl *createControl(INodeView *view) const;

protected:
	/* We need to remove all items and show only ".." */
	virtual void doesNotExistAnyMore() = 0;

protected:
	/* Tasks events */
	virtual UpdatesList::Map updateFilesMap() const = 0;
	virtual void updateFilesEvent(const UpdatesList &updates) = 0;
	virtual void scanForSizeEvent(bool canceled, const Snapshot &snapshot) = 0;
	virtual bool scanForCopyEvent(bool canceled, const Snapshot &snapshot, ICopyControl *control, bool move) = 0;
	virtual bool scanForRemoveEvent(bool canceled, const Snapshot &snapshot) = 0;
	virtual bool performCopyEvent(bool canceled, const Snapshot &snapshot, bool move) = 0;
	virtual void performRemoveEvent(bool canceled, const Snapshot &snapshot) = 0;

protected:
	/* Prepare tasks */
	void scanForSize(const TasksItemList &entries);
	void scanForCopy(const TasksItemList &entries, ICopyControl::Holder &destination, bool move);
	void scanForRemove(const TasksItemList &entries);
	void performCopy(BaseTask *oldTask, const Snapshot &Snapshot, IFileContainer::Holder &destination, bool move);
	void performRemove(BaseTask *oldTask, const Snapshot &Snapshot);

protected:
	bool isRoot() const { return m_info.isRoot(); }
	bool isUpdating() const { return m_updating; }
	void setUpdating(bool value) { m_updating = value; }
	void setInfo(const Info &info) { m_info = info; }

private:
	void updateFiles();
	void updateFiles(const BaseTask::Event *event);
	void scanForSize(const BaseTask::Event *event);
	void scanForCopy(const BaseTask::Event *event);
	void scanForRemove(const BaseTask::Event *event);
	void performCopy(const BaseTask::Event *event);
	void performRemove(const BaseTask::Event *event);

private:
	bool m_updating;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERNODEBASE_H_ */
