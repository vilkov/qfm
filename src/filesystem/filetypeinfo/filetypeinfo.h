#ifndef FILETYPEINFO_H_
#define FILETYPEINFO_H_

#include <QtGui/QIcon>

#if !defined(DESKTOP_ENVIRONMENT_IS_WINDOWS)
#	include "unix/filetypeid_unix.h"
	typedef ::FileSystem::FileTypeId_Unix FileTypeId;
#else
#	include "win32/filetypeid_win32.h"
	typedef ::FileSystem::FileTypeId_Win32 FileTypeId;
#endif


FILE_SYSTEM_NS_BEGIN

struct FileTypeInfo
{
	FileTypeInfo()
	{}

	bool isValid() const { return id.isValid(); }

	FileTypeId id;
	QIcon icon;
	QString name;
	QString descritpion;
};

FILE_SYSTEM_NS_END

#endif /* FILETYPEINFO_H_ */
