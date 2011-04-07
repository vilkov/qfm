#ifndef FILESTASK_H_
#define FILESTASK_H_

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QFileInfo>
#include "../filesysteminfo.h"
#include "../../../../tools/taskspool/task.h"


class FilesTask : public TasksPool::Task
{
public:
	FilesTask(const QString &directory, QObject *receiver);

    static FileSystemInfo info(const QFileInfo &fileInfo);

protected:
	const QString directory() const { return m_directory; }
	QObject *receiver() const { return m_receiver; }

	static QFile::Permissions translatePermissions(const QFileInfo &fileInfo);

private:
	QString m_directory;
	QObject *m_receiver;
#ifndef Q_OS_WIN
    uint m_userId;
    uint m_groupId;
#endif
};

#endif /* FILESTASK_H_ */
