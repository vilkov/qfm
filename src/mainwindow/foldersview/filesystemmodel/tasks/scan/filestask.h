#ifndef FILESTASK_H_
#define FILESTASK_H_

#include <QtCore/QString>
#include <QtCore/QFileInfo>
#include "../controlabletask.h"
#include "../../filesysteminfo.h"


class FilesTask : public ControlableTask
{
public:
	FilesTask(Params *params, QObject *controller1);
	FilesTask(Params *params, QObject *controller1, QObject *controller2);
	FilesTask(Params *params, QObject *controller1, QObject *controller2, QObject *controller3);

	static FileSystemInfo info(const QString &filePath);
	static FileSystemInfo info(const QFileInfo &fileInfo);

protected:
	static void updateInfo(FileSystemInfo &info);

	inline Params *parameters() const { return static_cast<Params*>(ControlableTask::parameters()); }

	FileSystemInfo getInfo(const QFileInfo &fileInfo) const;
#ifndef Q_OS_WIN
    static QFile::Permissions translatePermissions(const QFileInfo &fileInfo, uint userId, uint groupId);
#endif

private:
#ifndef Q_OS_WIN
    uint m_userId;
    uint m_groupId;
#endif
};

#endif /* FILESTASK_H_ */
