#ifndef FILESTASK_H_
#define FILESTASK_H_

#include <QtCore/QString>
#include <QtCore/QFileInfo>
#include "../../../../../filesysteminfo/filesysteminfo.h"


template <typename BaseClass>
class TemplateFilesTask : public BaseClass
{
public:
	typedef BaseClass parent_class;
	typedef typename parent_class::Params Params;

public:
	TemplateFilesTask(Params *params) :
		parent_class(params)
	#ifndef Q_OS_WIN
		,m_userId(getuid())
		,m_groupId(getgid())
	#endif
	{}


protected:
	FileSystemInfo getInfo(const QFileInfo &fileInfo) const
	{
	#ifdef Q_OS_WIN
	    return FileSystemInfo(fileInfo);
	#else
	    return FileSystemInfo(fileInfo, m_userId, m_groupId);
	#endif
	}

private:
#ifndef Q_OS_WIN
    uint m_userId;
    uint m_groupId;
#endif
};

#endif /* FILESTASK_H_ */
