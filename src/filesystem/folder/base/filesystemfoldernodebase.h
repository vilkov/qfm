#ifndef FILESYSTEMFOLDERNODEBASE_H_
#define FILESYSTEMFOLDERNODEBASE_H_

#include "containers/filesystemupdateslist.h"
#include "../../interfaces/filesystemifilecontainer.h"
#include "../../tasks/filesystemtasksnode.h"
#include "../../tasks/concrete/containers/filesystemsnapshot.h"


FILE_SYSTEM_NS_BEGIN

/*
 * Implements interaction with file system tasks (coping, moving, removing etc).
 *
 */

class FolderNodeBase : public TasksNode
{
	Q_DISABLE_COPY(FolderNodeBase)

public:
	FolderNodeBase(IFileContainer::Holder &container, const NodeModelContainer &items, Node *parent = 0);

	/* TasksNode */
    virtual bool event(QEvent *event);

    /* INode */
    virtual void refresh();
	virtual QString title() const;
	virtual QString location() const;
	virtual QString location(const QString &fileName) const;

    /* IFileOperations */
	virtual ICopyControl *createControl(INodeView *view) const;

protected:
	/* Tasks events */
	virtual UpdatesList::Changes updateFilesMap() const = 0;
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
	const IFileContainer *container() const { return m_container.data(); }

	//	bool isRoot() const { return m_info.isRoot(); }
	bool isUpdating() const { return m_updating; }
	void setUpdating(bool value) { m_updating = value; }
//	void setInfo(const Info &info) { m_info = info; }

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
	IFileContainer::Holder m_container;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERNODEBASE_H_ */
