#ifndef FILESYSTEMFOLDERNODEBASE_H_
#define FILESYSTEMFOLDERNODEBASE_H_

#include "containers/filesystemupdateslist.h"
#include "../../tasks/filesystemtasksnode.h"
#include "../../tasks/containers/filesystemscanedfiles.h"
#include "../../../tools/pointers/pscopedpointer.h"


FILE_SYSTEM_NS_BEGIN

/*
 * Implements interaction with file system tasks (coping, moving, removing etc).
 *
 */

class FolderNodeBase : public TasksNode
{
	Q_DISABLE_COPY(FolderNodeBase)

public:
	FolderNodeBase(const Info &info, const ModelContainer &conteiner, Node *parent = 0);

	/* TasksNode */
    virtual bool event(QEvent *event);

	/* IFileType */
	virtual QIcon icon() const;
	virtual QString name() const;
	virtual QString description() const;

	/* IFileInfo */
	virtual bool isDir() const;
	virtual bool isFile() const;
	virtual bool isLink() const;
	virtual bool exists() const;
	virtual qint64 fileSize() const;
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
	/* Tasks events */
	virtual UpdatesList::Map updateFilesMap() const = 0;
	virtual void updateFilesEvent(const UpdatesList &updates) = 0;
	virtual void scanForSizeEvent(bool canceled, const ScanedFiles &entries) = 0;
	virtual bool scanForCopyEvent(bool canceled, const ScanedFiles &entries, ICopyControl *control, bool move) = 0;
	virtual bool scanForRemoveEvent(bool canceled, const ScanedFiles &entries) = 0;
	virtual bool performCopyEvent(bool canceled, const ScanedFiles &entries, bool move) = 0;
	virtual void performRemoveEvent(const ScanedFiles &entries) = 0;

protected:
	/* Prepare tasks */
	void scanForSize(const TasksItemList &entries);
	void scanForCopy(const TasksItemList &entries, PScopedPointer<ICopyControl> &control, bool move);
	void scanForRemove(const TasksItemList &entries);
	void performCopy(BaseTask *oldTask, const ScanedFiles &entries, PScopedPointer<ICopyControl> &control, bool move);
	void performRemove(BaseTask *oldTask, const ScanedFiles &entries);

protected:
	bool isRoot() const { return m_info.isRoot(); }
	bool isUpdating() const { return m_updating; }
	void setUpdating(bool value) { m_updating = value; }
	void setInfo(const Info &info) { m_info = info; }

	IFileControl *create(const QString &name, IFileControl::FileType type, QString &error) const { return m_info.create(name, type, error); }

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
	Info m_info;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERNODEBASE_H_ */
