#ifndef FILESYSTEMFILETYPEINFO_H_
#define FILESYSTEMFILETYPEINFO_H_

#if !defined(DESKTOP_ENVIRONMENT_IS_WINDOWS)
#	include "unix/filesystemfiletypeid_unix.h"
	typedef FileTypeId_Unix FileTypeId;
#else
#	include "win32/filesystemfiletypeid_win32.h"
	typedef FileTypeId_Win32 FileTypeId;
#endif


struct FileTypeInfo
{
	FileTypeInfo();

	FileTypeId id;
	QIcon icon;
	QString name;
	QString descritpion;
};

#endif /* FILESYSTEMFILETYPEINFO_H_ */
