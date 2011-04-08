#ifndef FILESTASK_H_
#define FILESTASK_H_

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QFileInfo>
#include "../filesysteminfo.h"
#include "../items/filesystemtree.h"
#include "../../../../tools/taskspool/task.h"


class FilesTask : public TasksPool::Task
{
public:
	FilesTask(FileSystemTree *tree, const QString &directory, QObject *receiver);

	static FileSystemInfo info(const QFileInfo &fileInfo);

protected:
    FileSystemTree *tree() const { return m_tree; }
	const QString directory() const { return m_directory; }
	QObject *receiver() const { return m_receiver; }

    FileSystemInfo getInfo(const QFileInfo &fileInfo) const;
#ifndef Q_OS_WIN
    static QFile::Permissions translatePermissions(const QFileInfo &fileInfo, uint userId, uint groupId);
#endif

private:
    FileSystemTree *m_tree;
	QString m_directory;
	QObject *m_receiver;
#ifndef Q_OS_WIN
    uint m_userId;
    uint m_groupId;
#endif
};

#endif /* FILESTASK_H_ */
