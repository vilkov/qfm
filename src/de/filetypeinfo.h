#ifndef FILETYPEINFO_H_
#define FILETYPEINFO_H_

#if !defined(DESKTOP_ENVIRONMENT_IS_WINDOWS)
#	include "unix/filetypeid_unix.h"
	typedef FileTypeId_Unix FileTypeId;
#else
#	include "win32/filetypeid_win32.h"
	typedef FileTypeId_Win32 FileTypeId;
#endif


struct FileTypeInfo
{
	FileTypeInfo()
	{}

	FileTypeId id;
	QIcon icon;
	QString name;
	QString descritpion;
};

#endif /* FILETYPEINFO_H_ */
