#ifndef FILESYSTEMFOLDERNODEBASE_H_
#define FILESYSTEMFOLDERNODEBASE_H_

#include <QtCore/QStringList>
#include "events/filesystemmodelevent.h"
#include "containers/filesystemupdateslist.h"
#include "../../tasks/filesystemtasksnode.h"
#include "../../../tools/pointers/pscopedpointer.h"


FILE_SYSTEM_NS_BEGIN

/*
 * Implements general file system tasks (coping, moving, removing etc).
 *
 */

class FolderNodeBase : public TasksNode
{
	Q_DISABLE_COPY(FolderNodeBase)

public:
	FolderNodeBase(const Info &info, const ModelContainer &conteiner, Node *parent = 0);

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
	virtual void scanForSizeEvent(bool canceled, const InfoListItem *entries) = 0;
	virtual bool scanForCopyEvent(bool canceled, const InfoListItem *entries, ICopyControl *control, bool move) = 0;
	virtual bool scanForRemoveEvent(bool canceled, const InfoListItem *entries) = 0;
	virtual bool performCopyEvent(bool canceled, const InfoListItem *entries, bool move) = 0;
	virtual void performRemoveEvent(const InfoListItem *entries) = 0;

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
	void setInfo(const Info &info) { m_info = info; }

	IFileControl *create(const QString &name, IFileControl::FileType type, QString &error) const { return m_info.create(name, type, error); }

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
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERNODEBASE_H_ */
